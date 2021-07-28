/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include "cgnv2rt.h"
#include "cgnv2rtStr.h"

#include <cgbio.hpp>
#include <cgbutils.hpp>
#include <cgbtypes.h>

#include <Cg/CgInternal.h>

#include <stdlib.h>

using namespace cgc::bio;

//////////////////////////////////////////////////////////////////////////////
// internal types used for the conversion

//base type
class CgBaseType
{
public:
	CgBaseType() {_type = 0;}
	virtual ~CgBaseType() {}
	unsigned short _type; //index of the type in the types array of the program if >= STRUCT, else it's a BASETYPE
	short _resourceIndex; //index of the type in the types array of the program if >= STRUCT, else it's a BASETYPE
	unsigned short _resource; //index of the type in the types array of the program if >= STRUCT, else it's a BASETYPE
};

//array
class CgArrayType : public CgBaseType
{
public:
	CgArrayType():_elementType(NULL) { _type = (unsigned char)(CG_ARRAY + 128); }
	virtual ~CgArrayType()
	{
		if (_elementType)
			delete _elementType;
	}
	CgBaseType *_elementType;
	unsigned char _dimensionCount;
	unsigned short _dimensionItemCountsOffset;
};

//structure
class CgStructureType : public CgBaseType
{
public:
	//definitions
	class CgStructuralElement
	{
	public:
		char _name[256];
		char _semantic[256];
		CgBaseType *_type;
		unsigned short _flags; //in,out,global,varying,const,uniform,shared, hasDefaults(base types only) -> consecutive reg allocation flag
		unsigned short _index; //helper index ?
	};

	//implementation
	CgStructureType()
	{
		_type = (unsigned char)(CG_STRUCT + 128);
		_root = false;
	}

	virtual ~CgStructureType()
	{
		int i=0;
		int count = (int)_elements.size();
		for (i=0;i<count;i++)
		{
			if (_elements[i]._type)
				delete _elements[i]._type;
		}
	}

	//members
	STL_NAMESPACE vector<CgStructuralElement> _elements;
	bool _insideArray; //modifies the way the resources are stored, they should be sent directly to the resources,
	//the array is keeping track of the resource offset in that case
	bool _root;
};

typedef struct
{
	const char *name;
	int resIndex;
} BINDLOCATION;



//////////////////////////////////////////////////////////////////////////////
// static arrays containing type information

#define CG_DATATYPE_MACRO(name, compiler_name, enum_name, base_enum, nrows, ncols,classname) \
	classname ,
static int classnames[] = {
#include "Cg/cg_datatypes.h"
};

#define CG_DATATYPE_MACRO(name, compiler_name, enum_name, base_enum, nrows, ncols,classname) \
	nrows ,
static int rows[] = {
#include "Cg/cg_datatypes.h"
};

#ifdef JS_CG_CONVERTER_REPORT

#define CG_DATATYPE_MACRO(name, compiler_name, enum_name, base_enum, nrows, ncols,classname) \
#compiler_name ,
static const char *typenames[] = {
#include "Cg/cg_datatypes.h"
};

#define CG_BINDLOCATION_MACRO(name,enum_name,compiler_name,enum_int,addressable,param_type) \
{ compiler_name ,enum_int },

BINDLOCATION bindlocations[] =
{
#include <Cg/cg_bindlocations.h>
	{ "undefined",3256 },
	{ 0, 0 }
};
static const char *bindlocationsNames[4096];

extern bool _cmdQuietMode;
extern bool _cmdStripSemantics;
extern bool _cmdStripUnreferenced;

#define OUTPUT_TEXT(text) { if (!_cmdQuietMode) printf text; }

#endif

typedef float _float4[4];

#ifdef JS_CG_CONVERTER_REPORT
extern int _totalUcodeSize;
extern int _totalStringTableSize;
extern int _totalParametersSize;
extern int _fpCount;
extern int _vpCount;
extern int _totalParamCount;
extern int _totalLeaveParamCount;
#endif

//////////////////////////////////////////////////////////////////////////////
// static buffers containing the information extracted from the shaders

typedef struct
{
	STL_NAMESPACE vector<short> _resources;
	STL_NAMESPACE vector<unsigned short> _defaultValuesIndices;
	STL_NAMESPACE vector<unsigned short> _elfDefaultsIndices;
	STL_NAMESPACE vector<short> _dimensions;
	STL_NAMESPACE vector<CgParameterSemantic> _semanticIndices;
} _CGNVCONTAINERS;

static bool bIsVertexProgram = true;


//////////////////////////////////////////////////////////////////////////////
// forward declarations

static int getStride(CgBaseType *type);
static int getSizeofSubArray(_CGNVCONTAINERS &containers, int dimensionIndex, int dimensionCount, int endianness);
static unsigned int stringTableFind( STL_NAMESPACE vector<char> &stringTable, const char* str);
static unsigned int stringTableAdd( STL_NAMESPACE vector<char> &stringTable, const char* str );
static unsigned int stringTableAddUnique( STL_NAMESPACE vector<char> &stringTable, const char* str );
template<class Type> static size_t array_size(STL_NAMESPACE vector<Type> &array);
template<class Type> static void array_push(char* &parameterOffset, STL_NAMESPACE vector<Type> &array);
inline static Elf32_Word swap16(const Elf32_Word v);
static unsigned short getFlags(CGenum var, CGenum dir, int no,	bool is_referenced, bool is_shared, int paramIndex);

#ifdef JS_CG_CONVERTER_REPORT
static void debugPrintStructure(_CGNVCONTAINERS &containers, CgStructureType *structure, int endianness, STL_NAMESPACE vector<float> &defaultValues,int indent = 0);
#endif

//////////////////////////////////////////////////////////////////////////////
// implementation

static void fillStructureItems(_CGNVCONTAINERS &containers, CgStructureType *structure,
							   int endianness,
							   STL_NAMESPACE vector<CgParameterEntry> &parameterEntries,
							   STL_NAMESPACE vector<char> &parameterResources, STL_NAMESPACE vector<char> &stringTable, unsigned short *arrayResourceIndex = NULL,
							   unsigned short *arrayDefaultValueIndex = NULL);


int convertNvToElfFromFile(const char *sourceFile, int endianness, int constTableOffset, void **binaryShader, int *size,
						   STL_NAMESPACE vector<char> &stringTable, STL_NAMESPACE vector<float> &defaultValues)
{
#ifdef JS_CG_CONVERTER_REPORT
	OUTPUT_TEXT(("%s\n",sourceFile));
#endif
	FILE *fp = fopen(sourceFile, "rb");
	if (fp)
	{
		fseek(fp,0,SEEK_END);
		size_t filesize = ftell(fp);
		fseek(fp,0,SEEK_SET);
		void *data = malloc(filesize);
		if (data == NULL)
		{
			fclose(fp);
			JS_ASSERT2(0,("not enough memory to read the shader source"));
			return -2;
		}
		fread(data,filesize,1,fp);
		fclose(fp);

		int res = convertNvToElfFromMemory(data,filesize,endianness,constTableOffset, binaryShader, size,stringTable,defaultValues);
		free(data);
		return res;
	}
	JS_ASSERT2(0,("couldn't open source file %s\n",sourceFile));
	return -1;
}

int convertNvToElfFromMemory(const void *sourceData, size_t size, int endianness, int constTableOffset, void **binaryShader, int *binarySize,
							 STL_NAMESPACE vector<char> &stringTable, STL_NAMESPACE vector<float> &defaultValues)
{
	_CGNVCONTAINERS containers;

	unsigned char elfEndianness = endianness; //used in the macro CNVEND

	nvb_reader* nvbr = 0;
	bin_io::instance()->new_nvb_reader( &nvbr );
	CGBIO_ERROR err = nvbr->loadFromString((const char*)sourceData,size);
	if (err != CGBIO_ERROR_NO_ERROR)
		return -1;

#ifdef JS_CG_CONVERTER_REPORT
	OUTPUT_TEXT(( "%s\n", bin_io::instance()->error_string( err )));
	OUTPUT_TEXT(( "Profile %d, Revision %d, Size %d, Ucode %d\n",
		nvbr->profile(), nvbr->revision(), nvbr->size(), nvbr->ucode_size() ));
#endif

	//flag which tells if we have to swap data coming from nv and going to elf
	//bool doSwap = !(host_endianness() == (HOST_ENDIANNESS)elfEndianness);
	bool doSwap = !(nvbr->endianness() == (HOST_ENDIANNESS)elfEndianness);

	CGprofile NVProfile = nvbr->profile();
	//hack to counter removal of TypeC during beta
	if (NVProfile == (CGprofile)7005 )
		NVProfile = CG_PROFILE_SCE_VP_RSX;
	if (NVProfile == (CGprofile)7006 )
		NVProfile = CG_PROFILE_SCE_FP_RSX;
	if (NVProfile == CG_PROFILE_SCE_VP_TYPEB || NVProfile == CG_PROFILE_SCE_VP_RSX)
	{
		bIsVertexProgram = true;
#ifdef JS_CG_CONVERTER_REPORT
		_vpCount++;
#endif
	}
	else if (NVProfile == CG_PROFILE_SCE_FP_TYPEB || NVProfile == CG_PROFILE_SCE_FP_RSX)
	{
		bIsVertexProgram = false;
#ifdef JS_CG_CONVERTER_REPORT
		_fpCount++;
#endif
	}
	else
	{
		JS_ASSERT2(0,("error: unknown shader profile\n"));
		return -1;
	}

	//Fill the shader header structure and save it into the shadertab
	CgProgramHeader cgShader;
	memset(&cgShader,0,sizeof(CgProgramHeader));
	cgShader.profile = CNV2END((unsigned short) NVProfile); //ok cnv2end here we go from platform endiannes to elf endiannes
	cgShader.compilerVersion = 0;//TODO
	if (bIsVertexProgram)
	{
		const CgBinaryVertexProgram *nvVertex = nvbr->vertex_program();
		if (doSwap) //here we go directly from nv to elf endiannes without going to the platform endianness
		{
			cgShader.instructionCount = ENDSWAP(nvVertex->instructionCount);
			cgShader.attributeInputMask = ENDSWAP(nvVertex->attributeInputMask);
			//vertex program specific
			cgShader.vertexProgram.instructionSlot = ENDSWAP(nvVertex->instructionSlot);
			cgShader.vertexProgram.registerCount = ENDSWAP(nvVertex->registerCount);
			cgShader.vertexProgram.attributeOutputMask = ENDSWAP(nvVertex->attributeOutputMask);
			cgShader.vertexProgram.userClipMask = ENDSWAP(nvVertex->userClipMask);
		}
		else
		{
			cgShader.instructionCount = nvVertex->instructionCount;
			cgShader.attributeInputMask = nvVertex->attributeInputMask;
			//vertex program specific
			cgShader.vertexProgram.instructionSlot = nvVertex->instructionSlot;
			cgShader.vertexProgram.registerCount = nvVertex->registerCount;
			cgShader.vertexProgram.attributeOutputMask = nvVertex->attributeOutputMask;
			cgShader.vertexProgram.userClipMask = nvVertex->userClipMask;
		}
	}
	else
	{
		const CgBinaryFragmentProgram *nvFragment = nvbr->fragment_program();
		unsigned char flags;
		if (doSwap)
		{
			cgShader.instructionCount = ENDSWAP(nvFragment->instructionCount);
			cgShader.attributeInputMask = ENDSWAP(nvFragment->attributeInputMask);
			//fragment program specific
			cgShader.fragmentProgram.partialTexType = ENDSWAP(nvFragment->partialTexType);
			cgShader.fragmentProgram.texcoordInputMask = ENDSWAP(nvFragment->texCoordsInputMask);
			cgShader.fragmentProgram.texcoord2d = ENDSWAP(nvFragment->texCoords2D);
			cgShader.fragmentProgram.texcoordCentroid = ENDSWAP(nvFragment->texCoordsCentroid);
			cgShader.fragmentProgram.registerCount = ENDSWAP(nvFragment->registerCount);
			flags =
				(nvFragment->outputFromH0 ? CGF_OUTPUTFROMH0 : 0) |
				(nvFragment->depthReplace ? CGF_DEPTHREPLACE : 0) |
				(nvFragment->pixelKill ? CGF_PIXELKILL : 0);
		}
		else
		{
			cgShader.instructionCount = nvFragment->instructionCount;
			cgShader.attributeInputMask = nvFragment->attributeInputMask;
			//fragment program specific
			cgShader.fragmentProgram.partialTexType = nvFragment->partialTexType;
			cgShader.fragmentProgram.texcoordInputMask = nvFragment->texCoordsInputMask;
			cgShader.fragmentProgram.texcoord2d = nvFragment->texCoords2D;
			cgShader.fragmentProgram.texcoordCentroid = nvFragment->texCoordsCentroid;
			cgShader.fragmentProgram.registerCount = nvFragment->registerCount;
			flags =
				(nvFragment->outputFromH0 ? CGF_OUTPUTFROMH0 : 0) |
				(nvFragment->depthReplace ? CGF_DEPTHREPLACE : 0) |
				(nvFragment->pixelKill ? CGF_PIXELKILL : 0);
		}
		cgShader.fragmentProgram.flags = CNV2END(flags);
	}

	//shader specific info ( the shader header is in the shader tab )

	//ucode
	Elf32_Word *tmp = (Elf32_Word *)nvbr->ucode();
	const char *ucode;
	Elf32_Word *buffer = NULL;
	if (doSwap)
	{
		int size = (int)nvbr->ucode_size()/sizeof(Elf32_Word);
		buffer = new Elf32_Word[size];
		for (int i=0;i<size;i++)
		{
			Elf32_Word val = ENDSWAP(tmp[i]);
			if (!bIsVertexProgram)
				val = swap16(val);
			buffer[i] = val;
		}
		ucode = (const char*)buffer;
	}
	else
	    {
		ucode = (const char*)tmp;
		// !!!xxx this is to workaround what appears to be a linux platform specific bug
		// that manifests as a memory overwrite in properly allocated memory during a STL_NAMESPACE vector resize
		int size = (int)nvbr->ucode_size()/sizeof(Elf32_Word);
		buffer = new Elf32_Word[size];
		for (int i=0;i<size;i++)
		{
			buffer[i] = tmp[i];
		}
		ucode = (const char*)buffer;
		// end workaround
	    }

	int ucodeSize = nvbr->ucode_size();

	//ucode, ucodeSize;

	JS_ASSERT2(CNV2END(cgShader.instructionCount)*16 ==nvbr->ucode_size(),("%i, %i",CNV2END(cgShader.instructionCount)*16,nvbr->ucode_size()));
#ifdef JS_CG_CONVERTER_REPORT
	_totalUcodeSize += ucodeSize;
#endif

	//parameters
	CgStructureType root;
	root._insideArray = false;
	root._root = true;

	//we don't have the names of the user structures, so to share structures we will have to rely on the
	//layout of the data, we might do mistakes, but it's not important, since what we want to represent is
	//the structure layout.
	//parameters
	//the parameters represents a hierarchical structure, I am reconstructing this structure

	int paramIndex = -1;

	//just for sanity check purpose: it's use to detect when there's a change of structural element to reset the embedded constant counter
	CgStructureType::CgStructuralElement *current = NULL;
	int embeddedConstants = 0;
	bool rootChildDefaultValues = false;
	int rootChildIndex = -1;

	char currentRefStructureName[256];
	currentRefStructureName[0] = '\0';

	int i;
	for (i = 0; i < (int)nvbr->number_of_params(); i++)
	{
		CGtype type;
		CGresource res;
		CGenum var;
		int rin;
		const char *name;
		STL_NAMESPACE vector<float> dv;
		STL_NAMESPACE vector<unsigned int> ec;
		const char *sem;
		CGenum dir;
		int no;
		bool is_referenced;
		bool is_shared;
		nvbr->get_param( i, type, res, var, rin, &name, dv, ec, &sem, dir, no, is_referenced, is_shared );

#ifdef JS_CG_CONVERTER_REPORT

		//strip unreferenced parameters
		if (_cmdStripUnreferenced)
		{
			bool differentStructureName = (strncmp(currentRefStructureName,name,strlen(currentRefStructureName)) != 0);
			if (!is_referenced && differentStructureName)
			{
				char *endRefStructName = strchr(name,'[');
				if (!endRefStructName)
					continue;

				char buffer[512];
				size_t namelength = endRefStructName - name + 1; //take the '['
				strncpy(buffer,name,namelength);
				buffer[namelength] = '\0';

				//we can have a matrix or an array here
				//case of arrays, when we have an array, we receive the items one by one, however if at least one further
				//array item is referenced, we need to keep the full array
				//so I need to double check here that we don't have an array
				int j = i+1;
				bool keepObject = false;
				while (j < (int)nvbr->number_of_params())
				{
					const char *name2;
					bool is_referenced;
					nvbr->get_param_name(j,&name2,is_referenced);

					if (strncmp(buffer,name2,namelength)!=0) //different object
						break;

					if (is_referenced)
					{
						keepObject = true;
						break;
					}
					j++;
				}
				if (!keepObject)
					continue;
			}

			char *endRefStructName = strchr(name,'[');
			if (endRefStructName)
			{
				size_t length = endRefStructName - name;
				strncpy(currentRefStructureName,name,length);
				currentRefStructureName[length] = '\0';
			}
		}

		_totalParamCount++;
		_totalLeaveParamCount++;
		if (classnames[type - CG_TYPE_START_ENUM-1] == CG_PARAMETERCLASS_MATRIX)
			_totalLeaveParamCount-=4;
#endif

		/*printf( "%2d: name:%14s, semantic:%10s, type:%4d\n    variability:%4d, direction:%4d, resource:%4d, resource_ndex:%2d\n    paramNo:%4d, referenced:%1d, shared:%1d\n",
		i, name, sem, type, var, dir, res, rin, no, is_referenced, is_shared );
		for ( int jj = 0; jj < (int)dv.size(); ++jj )
		printf( ", dv[%2d] %g", jj, dv[jj] );
		if (dv.size()) printf("\n");
		printf("\n");*/

		//This code is there twice,
		//here it fixes a sce-cgc "bug" or weak feature, the user has asked for a shared parameter, but the flag is not set
		//it happens for unreferenced item in a structure
		//it will break if we support unshared with the semantic 'C###'
		//later it will be there to fix the contiguity for arrays and the "share" part, but it's actually 2 different problems.
		if (strlen(sem)>=2 && sem[0] == 'C')
		{
			const char *szSem = sem+1;
			//check if we have a number
			while (*szSem != '\0')
			{
				if ( (*szSem) < '0' || (*szSem) > '9')
					break;
				szSem++;
			}
			if (*szSem == '\0')
				is_shared = 1;
		}

		const char *parameterName = name;
		const char *structureEnd = NULL;
		CgStructureType *container = &root;
		int done = 0;
		const char * structureStart = parameterName;
		while (!done && *structureStart)
		{
			// process the next structural element
			structureEnd = strpbrk(structureStart, ".[");

			if (structureEnd)
			{
				//there's a special case for sampler, we don't support arrays of sampler, so unrolled the array here
				//and don't even register the structure as being an array
				if (*structureEnd == '[' && type >= CG_SAMPLER1D && type <= CG_SAMPLERCUBE)
				{
					//do we have an array of samplers and are we at the end ?
					const char *closed = strchr(structureEnd, ']');
					JS_ASSERT2(closed,("name error for parameter %s",parameterName));
					const char *somethingElse = strpbrk(closed, ".[");
					if (!somethingElse)
						structureEnd = NULL; //so we are at the end we are in an array of samplers
				}
			}

			// test if we are done finding structural information
			if (structureEnd == NULL)
			{
				//set structureEnd correctly so that the rest of the function performs correctly
				structureEnd = structureStart + strlen(structureStart);
				//set the done flag to exit the loop
				done = 1;
			}

			//extract the current structural element name
			char structName[256];
			int length = (int)(structureEnd - structureStart);
			strncpy(structName, structureStart, length);
			structName[length] = '\0';

			//do we already have it in the structure ?
			CgStructureType::CgStructuralElement *structuralElement = NULL;
			int j=0;
			int elementCount = (int)container->_elements.size();
			for (j=0;j<elementCount;j++)
			{
				structuralElement = &container->_elements[j];
				if (!strcmp(structuralElement->_name,structName))
				{
					//same name, we need to check that this is the same scope ( program versus global )
					if ( (no == -1 && (structuralElement->_flags & CGPF_GLOBAL)) ||
						 (no != -1 && !(structuralElement->_flags & CGPF_GLOBAL)))
						break;
				}
			}
			if (j==elementCount)
			{
				if (container == &root)
				{
					if (rootChildIndex != j)
					{
						rootChildDefaultValues = false;
						rootChildIndex = j;
					}
				}

				//we don't have it yet, add it
				container->_elements.resize(elementCount+1);
				structuralElement = &container->_elements[elementCount];
				strncpy(structuralElement->_name,structName,sizeof(structuralElement->_name));
				structuralElement->_name[sizeof(structuralElement->_name)-1] = '\0';
				structuralElement->_flags = getFlags(var,dir,no,is_referenced,is_shared,paramIndex);
				int dimensionCount = 0;

				if (strncmp(sem, "COLOR", 5) == 0 || strncmp(sem, "NORMAL", 6) == 0)
					structuralElement->_flags |= CGP_NORMALIZE;


				//check if we have a structure or an array of structure or an array of basetypes
				//then:
				// - create an array type if we have an array
				// - update the container if we have a structure or an array of structure
				int isStructure = (*structureEnd == '.');
				if (*structureEnd == '[')
				{
					dimensionCount++;
					const char *arrayEnd = strchr(structureEnd,']')+1;
					while (*arrayEnd == '[')
					{
						arrayEnd = strchr(arrayEnd,']')+1;
						dimensionCount++;
					}
					if (*arrayEnd == '.')
						isStructure = true;
				}

				//we have an array, create an array type
				if (dimensionCount)
				{
					CgArrayType *arrayType = new CgArrayType;
					arrayType->_dimensionCount = dimensionCount;
					arrayType->_dimensionItemCountsOffset = (unsigned short)containers._dimensions.size();
					//initialize the dimension count;
					int k;
					for (k=0;k<dimensionCount;k++)
						containers._dimensions.push_back(CNV2END((short)0));
					structuralElement->_type = arrayType;
					if (isStructure)
					{
						//we have a structure inside the array
						container = new CgStructureType;
						container->_insideArray = true;
						arrayType->_elementType = container;
					}
					else
					{
						//we have a base type inside the array
						arrayType->_elementType = new CgBaseType;
						arrayType->_elementType->_type = type - CG_TYPE_START_ENUM;
					}
					arrayType->_elementType->_resource = res;
					arrayType->_elementType->_resourceIndex = -1;
					//special case here for shared vertex array ??? I consider them contiguous and referenced
					//I need to detect this case and it's complex
					if (bIsVertexProgram && strlen(sem)>=2 && sem[0] == 'C')
					{
						//I have to parse the semantic !!!! that's the only info I have at that level from sce-cgc...
						//TODO: it's going to break if we support unshared with the semantic 'C###'
						const char *szSem = sem+1;
						//check if we have a number
						while (*szSem != '\0')
						{
							if ( (*szSem) < '0' || (*szSem) > '9')
								break;
							szSem++;
						}
						if (*szSem == '\0')
						{
							//fix the is_shared variable.... sce-cgc doesn't set it that's why I am doing this parsing
							is_shared = 1;
							int registerIndex = atoi(sem+1);
							structuralElement->_flags |= CGP_CONTIGUOUS;
							structuralElement->_flags |= CGPF_SHARED;
							structuralElement->_type->_resourceIndex = registerIndex;
						}
						else
							structuralElement->_type->_resourceIndex = (int)containers._resources.size();
					}
					else
						structuralElement->_type->_resourceIndex = (int)containers._resources.size();

					structuralElement->_type->_resource = res;
				}
				else
				{
					//we have a structure
					if (isStructure)
					{
						bool insideArray = container->_insideArray;
						container = new CgStructureType;
						container->_insideArray = insideArray;
						structuralElement->_type = container;
					}
					else
					{
						//assign the basetype and resource
						structuralElement->_type = new CgBaseType;
						structuralElement->_type->_type = type - CG_TYPE_START_ENUM;
						structuralElement->_type->_resource = res;
						if (classnames[structuralElement->_type->_type-1] == CG_PARAMETERCLASS_MATRIX)
						{
							//TODO, Cedric: for vertexprogram assume the matrices are always fully allocated and that they have consecuttive assignements
							if (bIsVertexProgram)
							{
								structuralElement->_type->_resourceIndex = (short)rin;
							}
							else
								structuralElement->_type->_resourceIndex = (int)containers._resources.size();
						}
						else
						{
							if (!container->_insideArray)
							{
								//vertex program
								if (bIsVertexProgram)
									structuralElement->_type->_resourceIndex = rin;
								else
								{
									//fragment program
									if (structuralElement->_flags & CGPV_VARYING)
										structuralElement->_type->_resourceIndex = -1;
									else
									{
										structuralElement->_type->_resourceIndex = (int)containers._resources.size();
										containers._resources.push_back(CNV2END((unsigned short)rin));
										int size = (int)ec.size();
										containers._resources.push_back(CNV2END((unsigned short)size));
										int k;
										for (k=0;k<size;k++)
											containers._resources.push_back(CNV2END((unsigned short)ec[k]));
									}
								}
							}
							else
							{
								//will be done later
								structuralElement->_type->_resourceIndex = (short)-1;
								structuralElement->_type->_resource = (unsigned short)res;
							}
						}
					}
				}
			}
			else
			{
				if (structuralElement->_type->_type ==  CG_STRUCT+128)
				{
					container = (CgStructureType*)structuralElement->_type;
				}
				else if (structuralElement->_type->_type ==  CG_ARRAY+128)
				{
					CgArrayType *arrayType = (CgArrayType *)structuralElement->_type;
					if (arrayType->_elementType->_type >128 )
					{
						if (arrayType->_elementType->_type != CG_STRUCT+128) //we can't have arrays of arrays
						{
							JS_ASSERT2(0,("arrays of arrays not supported"));
						}
						container = (CgStructureType*)arrayType->_elementType;
					}
				}
			}

			//default values should only be at the root of the hierarchical parameter structure
			//JS_ASSERT2(!rootChildDefaultValues || dv.size(),("all the parameter below a root child should have a default value"));
			//if (dv.size() && !rootChildDefaultValues)
			if (dv.size())
			{
				int  size = (int)containers._defaultValuesIndices.size();
				if (!size  || (containers._defaultValuesIndices[size-2] != CNV2END((unsigned short)(rootChildIndex))))
				{
					//assert for default values on structures:
					//now supported:
					/*JS_ASSERT2(structuralElement->_type->_type != CG_STRUCT + 128,("default values on structure not yet supported"));
					JS_ASSERT2(structuralElement->_type->_type != CG_ARRAY + 128 ||
						((CgArrayType *)structuralElement->_type)->_elementType->_type != CG_STRUCT + 128,
						("default values on arrays of structures not yet supported"));*/

					//push the index of the parameter in the global structure
					containers._defaultValuesIndices.push_back(CNV2END((unsigned short)(rootChildIndex)));
					//push the index of the default value ( could add the size if necessary )
					containers._defaultValuesIndices.push_back(CNV2END((unsigned short)defaultValues.size()));
				}
			}

			//this case is for when we have item within a struct which is in an array, we don't duplicate the struct
			//but we still need to save the resources at the array level, so with this code when we reach the end of the parameter
			//name, we only need to save the resources
			if (container->_insideArray && done)
			{
				//the reason of the reset of the flag is because the first item of the array might unreferenced
				//so it doesn't has the correct resource type
				if (is_referenced)
				{
					bool sharedContiguous = (structuralElement->_flags & CGPF_SHARED) && (structuralElement->_flags & CGP_CONTIGUOUS);
					structuralElement->_flags = getFlags(var,dir,no,is_referenced,is_shared,paramIndex);
					//special case here for shared vertex array ??? I consider them contiguous and referenced
					if (sharedContiguous) //reset the contiguous flag too then
						structuralElement->_flags |= ( CGP_CONTIGUOUS | CGPF_SHARED);
				}
				if (bIsVertexProgram)
				{
					//special case here for shared vertex array ??? I consider them contiguous and referenced
					if (!is_shared) //if shared it's contiguous and we don't have to do that
						containers._resources.push_back(CNV2END((unsigned short)rin));
				}
				else
				{
					//fragment program
					if (structuralElement->_flags & CGPV_VARYING)
						containers._resources.push_back(CNV2END((unsigned short)rin));
					else
					{
						containers._resources.push_back(CNV2END((unsigned short)rin));
						int size = (int)ec.size();
						containers._resources.push_back(CNV2END((unsigned short)size));
						int k;
						for (k=0;k<size;k++)
							containers._resources.push_back(CNV2END((unsigned short)ec[k]));
					}
				}
			}

			//if we have an array we need to update the item counts and skip the array for the parsing of the name of the structure
			CgArrayType *arrayType = NULL;
			if (*structureEnd == '[')
			{
				int arrayCellIndex = 0;
				const char *arrayStart = structureEnd;
				const char *arrayEnd = structureEnd;
				CgBaseType *itemType = structuralElement->_type;
				if (itemType->_type >= 128)
				{
					arrayType = (CgArrayType *)itemType;
					int dimensionCount = 0;
					while (*arrayStart == '[' && dimensionCount<arrayType->_dimensionCount)
					{
						arrayEnd = strchr(arrayStart+1,']');
						int length =(int)(arrayEnd - arrayStart - 1);
						char indexString[16];
						strncpy(indexString,arrayStart+1,length);
						indexString[length] = '\0';
						int index = atoi(indexString);
						int dim = CNV2END(containers._dimensions[arrayType->_dimensionItemCountsOffset + dimensionCount]);
						if ((index+1) > dim)
							containers._dimensions[arrayType->_dimensionItemCountsOffset + dimensionCount] = CNV2END((short)(index+1));
						arrayCellIndex += index*getSizeofSubArray(containers,arrayType->_dimensionItemCountsOffset + dimensionCount,arrayType->_dimensionCount - dimensionCount -1,endianness);
						arrayStart = arrayEnd+1;
						dimensionCount++;
					}
					structureEnd = arrayStart;

					itemType = arrayType->_elementType;
					if (itemType->_type<128)
					{
						int rowCount = rows[itemType->_type-1];
						if (!rowCount) //no matrix here
						{
							//special case here for shared vertex array ??? I consider them contiguous and referenced
							//so I add this if and don't add the resource if we have a shared here
							//this is part of a big workaround around sce_cgc ...
							bool sharedContiguous = (structuralElement->_flags & CGPF_SHARED) && (structuralElement->_flags & CGP_CONTIGUOUS);
							if (!bIsVertexProgram || !sharedContiguous)
							{
								//add the resource index ... scary
								containers._resources.push_back(CNV2END((unsigned short)rin));
							}

							//fragment program
							if (!bIsVertexProgram)
							{
								int size = (int)ec.size();
								containers._resources.push_back(CNV2END((unsigned short)size));
								int k;
								for (k=0;k<size;k++)
									containers._resources.push_back(CNV2END((unsigned short)ec[k]));
							}
							JS_ASSERT2(*arrayStart == '\0',("internal error: matrix type expected"));
							done = 1;
						}
					}

					if (*arrayStart == '\0')
					{
						done = 1;
					}
				}

				//if we still have some [ , it means we have a matrix
				if (*structureEnd == '[')
				{
					int dimensionCount = 0;
					while (*arrayStart == '[')
					{
						arrayEnd = strchr(arrayStart+1,']');
						if (itemType->_type <128)//matrix
						{
							if (structuralElement != current)
							{
								embeddedConstants = 0;
								current = structuralElement;
							}

#ifdef JS_DEBUG
							int classname = classnames[itemType->_type-1];
							JS_ASSERT2(classname == CG_PARAMETERCLASS_MATRIX,("matrix expected"));
							JS_ASSERT2(dimensionCount==0,("matrices are vectors of floats4 , so dimension should be 1"));
#endif
							int length =(int)(arrayEnd - arrayStart - 1);
							char indexString[16];
							strncpy(indexString,arrayStart+1,length);
							indexString[length] = '\0';
#ifdef JS_DEBUG
							int index = atoi(indexString);
#endif

							//TODO: Cedric: for vertex program and matrices not in array, I am not adding any resource:
							if (bIsVertexProgram)
							{
								if (arrayType)
								{
									//special case here for shared vertex array ??? I consider them contiguous and referenced
									//so I add this if and don't add the resource if we have a shared here
									//this is part of a big workaround around sce_cgc ...
									bool sharedContiguous = (structuralElement->_flags & CGPF_SHARED) && (structuralElement->_flags & CGP_CONTIGUOUS);
									if (!sharedContiguous)
									{
										JS_ASSERT2((int)containers._resources.size() == structuralElement->_type->_resourceIndex + arrayCellIndex*rows[itemType->_type-1] + embeddedConstants + index,("matrix index mismatch. some indices have been skipped"));
										//add the resource index ... scary
										containers._resources.push_back(CNV2END((unsigned short)rin));
									}
								}
							}
							else
							{
								containers._resources.push_back(CNV2END((unsigned short)rin));
								int size = (int)ec.size();
								containers._resources.push_back(CNV2END((unsigned short)size));
								int k;
								for (k=0;k<size;k++)
									containers._resources.push_back(CNV2END((unsigned short)ec[k]));
								embeddedConstants += k+1;
							}
							JS_ASSERT2(*(arrayEnd+1) == '\0',("internal error: matrix type expected, the parameter specifier is longer than expected"));
							done = 1;
						}
						arrayStart = arrayEnd+1;
						dimensionCount++;
					}
					structureEnd = arrayEnd;
				}

				// matrix single or array or array of matrix: the flags should be the same for all the referenced items
				if (is_referenced)
				{
					bool sharedContiguous = (structuralElement->_flags & CGPF_SHARED) && (structuralElement->_flags & CGP_CONTIGUOUS);

					unsigned short flag = getFlags(var,dir,no,is_referenced,is_shared,paramIndex);
					structuralElement->_flags = flag;

					//special case here for shared vertex array ??? I consider them contiguous and referenced
					if (sharedContiguous) //reset the contiguous flag too then
						structuralElement->_flags |= ( CGP_CONTIGUOUS | CGPF_SHARED);

					structuralElement->_type->_resource = res;
					//if array
					if (arrayType)
						arrayType->_elementType->_resource = res;
				}
			}


			if (done && dv.size())
			{
				//very difficult to check that there is no mistake done... should do a diff with
				//containers._defaultValuesIndices[containers._defaultValuesIndices.size()-1] and the current parameter position in
				//the structure
				for ( int jj = 0; jj < (int)dv.size(); ++jj )
				{
#ifndef JS_PLATFORM_CELL
					unsigned int val = *(unsigned int*)&dv[jj];
					unsigned int tmp = CNV2END(val);
					float tmp2 = *(float*)&tmp;
					defaultValues.push_back(tmp2);
#else
					defaultValues.push_back(dv[jj]);
#endif
				}
			}

			if (done)
			{
#ifdef JS_CG_CONVERTER_REPORT
				if (strlen(sem) && !_cmdStripSemantics)
#else
				if (strlen(sem))
#endif
				{
					strncpy(structuralElement->_semantic,sem,sizeof(structuralElement->_semantic));
				}
				else
					structuralElement->_semantic[0] = '\0';
			}
			structureStart = structureEnd+1;
		}
	}

	//we are done with the reader
	nvbr->release();
	//release singleton
	bin_io::delete_instance();

	//print the structure that we have extracted

#ifdef JS_CG_CONVERTER_REPORT
	//fill the bind locations
	if (!_cmdQuietMode)
	{
		int bindlocationcount = (int)(sizeof(bindlocations) / sizeof(bindlocations[0]));
		for (i=0;i<bindlocationcount;i++)
			bindlocationsNames[bindlocations[i].resIndex] = bindlocations[i].name;
		debugPrintStructure(containers,&root, endianness,defaultValues);
	}
#endif

	//transform the extracted structure into the file format
	STL_NAMESPACE vector<CgParameterEntry> parameterEntries;
	STL_NAMESPACE vector<char> parameterResources;
	fillStructureItems(containers,&root,endianness,parameterEntries,parameterResources,stringTable);

	//save it
	CgParameterTableHeader header;
	memset(&header,0,sizeof(CgParameterTableHeader));
	header.entryCount = (unsigned short)parameterEntries.size();
	header.resourceTableOffset = sizeof(CgParameterTableHeader) + (unsigned short)(parameterEntries.size()*sizeof(parameterEntries[0]) + parameterResources.size()*sizeof(parameterResources[0]));
	header.defaultValueIndexTableOffset = header.resourceTableOffset + (unsigned short)containers._resources.size() * sizeof(containers._resources[0]);
	header.defaultValueIndexCount = (unsigned short)containers._elfDefaultsIndices.size()/2;
	header.semanticIndexTableOffset = header.defaultValueIndexTableOffset + (unsigned short)containers._elfDefaultsIndices.size() * sizeof (containers._elfDefaultsIndices[0]);
	header.semanticIndexCount = (unsigned short)containers._semanticIndices.size();

	header.entryCount = CNV2END(header.entryCount);
	header.resourceTableOffset = CNV2END(header.resourceTableOffset);
	header.defaultValueIndexTableOffset = CNV2END(header.defaultValueIndexTableOffset);
	header.defaultValueIndexCount = CNV2END(header.defaultValueIndexCount);
	header.semanticIndexTableOffset = CNV2END(header.semanticIndexTableOffset);
	header.semanticIndexCount = CNV2END(header.semanticIndexCount);

	//fill the parameter section
	size_t parameterTableSize = sizeof(CgParameterTableHeader);
	parameterTableSize += array_size(parameterEntries);
	parameterTableSize += array_size(parameterResources);
	parameterTableSize += array_size(containers._resources);
	//parameterTableSize += array_size(containers._defaultValuesIndices);
	parameterTableSize += array_size(containers._elfDefaultsIndices);
	parameterTableSize += array_size(containers._semanticIndices);

	//Allocate the binary file
	int ucodeOffset = (((sizeof(CgProgramHeader)-1)/16)+1)*16;
	size_t programSize = ucodeOffset + ucodeSize + parameterTableSize;
	char *program = new char[programSize];

	//header
	memcpy(program,&cgShader,sizeof(CgProgramHeader));
	if (ucodeOffset-sizeof(CgProgramHeader))
		memset(program+sizeof(CgProgramHeader),0,ucodeOffset-sizeof(CgProgramHeader));

	//ucode
	memcpy(program + ucodeOffset,ucode,ucodeSize);
	//we can delete buffer ( memory block pointed by ucode )
	if (!bIsVertexProgram && doSwap)
		delete[] buffer;
	// !!!xxx this is to workaround what appears to be a linux platform specific bug
	// that manifests as a memory overwrite in properly allocated memory during a STL_NAMESPACE vector resize
	else
	    delete[] buffer;
	// end workaround

	//parameters
	char *parameterOffset = program + ucodeOffset + ucodeSize;

	memcpy(parameterOffset,&header,sizeof(CgParameterTableHeader));
	parameterOffset += sizeof(CgParameterTableHeader);
	array_push(parameterOffset, parameterEntries);
	array_push(parameterOffset, parameterResources);
	array_push(parameterOffset, containers._resources);
	//TODO: Tmp: patch the default Value indices, no sharing yet
	/*unsigned short offset = (unsigned short)constTableOffset;
	for (int i=0;i<(int)containers._defaultValuesIndices.size();i+=2)
	{
		unsigned short value = CNV2END(containers._defaultValuesIndices[i+1]);
		containers._defaultValuesIndices[i+1] = CNV2END((unsigned short)(value + offset));
	}*/
	//array_push(parameterOffset, containers._defaultValuesIndices);
	array_push(parameterOffset, containers._elfDefaultsIndices);
	array_push(parameterOffset, containers._semanticIndices);

	JS_ASSERT2(parameterOffset == program + programSize,("error\n"));
	//set the return values
	*binarySize = (int)programSize;
	*binaryShader = program;

	return 0;
}

int convertNvToElfFreeBinaryShader(void *binaryShader)
{
	char *program = (char *)binaryShader;
	delete[] program;
	return 0;
}

static void pushbackUnsignedShort(STL_NAMESPACE vector<char> &parameterResources, unsigned short value)
{
	size_t size = parameterResources.size();
	parameterResources.resize(size + 2);
	*((unsigned short*)&parameterResources[size]) = value;
}

//This function fill the parameter tables that will be written to the files
static void fillStructureItems(_CGNVCONTAINERS &containers, CgStructureType *structure, int endianness,
						STL_NAMESPACE vector<CgParameterEntry> &parameterEntries,STL_NAMESPACE vector<char> &parameterResources,
						STL_NAMESPACE vector<char> &stringTable, unsigned short *arrayResourceIndex, unsigned short *arrayDefaultValueIndex)
{
	unsigned char elfEndianness = endianness; //used in the macro CNVEND

	int currentDefaultIndex = 0;
	int count = (int)structure->_elements.size();
	int i;
	for (i=0;i<count;i++)
	{
		CgStructureType::CgStructuralElement *structuralElement = &structure->_elements[i];
		size_t size = parameterEntries.size();
		parameterEntries.resize(size+1);
		CgParameterEntry *parameterEntry = &parameterEntries[size];
		parameterEntry->nameOffset = CNV2END((int)stringTableAddUnique(stringTable, structuralElement->_name));
		if (structuralElement->_semantic[0])
		{
			CgParameterSemantic semantic;
			semantic.entryIndex = CNV2END((unsigned short)size);
			semantic.reserved = 0;
			semantic.semanticOffset = CNV2END((int)stringTableAddUnique(stringTable, structuralElement->_semantic));
			containers._semanticIndices.push_back(semantic);
		}
		parameterEntry->flags = CNV2END(structuralElement->_flags);
		//prepare the typeIndex
		unsigned short typeIndex = ((unsigned short)parameterResources.size());
		JS_ASSERT2((typeIndex&3) == 0,("typeindex must be aligned on 4 bytes"));
		parameterEntry->typeIndex = CNV2END(typeIndex);

		CgBaseType *itemType = structuralElement->_type;
		unsigned short _resource = itemType->_resource;
		unsigned short _resourceIndex = itemType->_resourceIndex;

		//will contain the parameterEntry for this item, for simple type arrays the value will be corrected
		//parameterEntry.size() is not parameterEntryIndex in that case.
		int parameterEntryIndex;

		if (itemType->_type-128 == CG_ARRAY)
		{
			CgArrayType *arrayType = (CgArrayType *)structuralElement->_type;
			int arraySize = getSizeofSubArray(containers,arrayType->_dimensionItemCountsOffset,arrayType->_dimensionCount,endianness);
			itemType = arrayType->_elementType;
			JS_ASSERT2(!(itemType->_type +CG_TYPE_START_ENUM >= CG_SAMPLER1D && itemType->_type +CG_TYPE_START_ENUM<= CG_SAMPLERCUBE),("array of samplers not yet supported"));
			unsigned short arrayFlag = CGP_ARRAY;
			parameterEntry->flags |= CNV2END(arrayFlag);
			parameterResources.resize(typeIndex+sizeof(CgParameterArray));

			CgParameterArray *parameterArray = (CgParameterArray *)(&parameterResources[typeIndex]);
			if (itemType->_type-128 == CG_STRUCT )
				parameterArray->arrayType = CNV2END((unsigned short)CG_STRUCT);
			else
				parameterArray->arrayType = CNV2END(itemType->_type+128);
			parameterArray->dimensionCount = CNV2END((unsigned short)arrayType->_dimensionCount);
			int j;
			for (j=0;j<arrayType->_dimensionCount;j++)
			{
				pushbackUnsignedShort(parameterResources,(unsigned short)containers._dimensions[arrayType->_dimensionItemCountsOffset+j]); //already endian-swapped
			}
			//padding
			if (arrayType->_dimensionCount&1)
				pushbackUnsignedShort(parameterResources,CNV2END((unsigned short)0));

			//unroll the array of structure here, create one structure for each item of the array
			if (itemType->_type-128 == CG_STRUCT )
			{
				unsigned short unrolledFlag  = CGP_UNROLLED;
				parameterEntry->flags |= CNV2END(unrolledFlag);
				CgStructureType *structureType = (CgStructureType*)itemType;
				int k;

				//those lines won't work for arrays of structures containing arrays
				unsigned short _arrayResourceIndex = (unsigned short)(arrayType->_resourceIndex);
				unsigned short _arrayDefaultValueIndex = 0;

				bool hasDefaults = false;
				if (structure->_root && containers._defaultValuesIndices.size())
				{
					if (containers._defaultValuesIndices[currentDefaultIndex*2] == CNV2END((unsigned short)i))
					{
						hasDefaults = true;
						_arrayDefaultValueIndex = containers._defaultValuesIndices[currentDefaultIndex*2+1];
						//containers._defaultValuesIndices[currentDefaultIndex*2] = CNV2END((unsigned short)(parameterEntries.size() - 1));
						//currentDefaultIndex++;
					}
				}

				for (k=0;k<arraySize;k++)
				{
					size_t size = parameterEntries.size();
					parameterEntries.resize(size+1);

					CgParameterEntry &parameterArrayEntry = parameterEntries[size];

					char buffer[256];
					sprintf(buffer,"%s[%i]",structuralElement->_name,k);
					parameterArrayEntry.nameOffset = CNV2END((int)stringTableAddUnique(stringTable, buffer));
					parameterArrayEntry.flags = CNV2END(structuralElement->_flags);
					unsigned short structureFlag = CGP_STRUCTURE;
					parameterArrayEntry.flags |= CNV2END(structureFlag);

					unsigned short arrayEntryTypeIndex = (unsigned short)parameterResources.size();
					JS_ASSERT2((arrayEntryTypeIndex&3) == 0,("typeindex must be aligned on 4 bytes"));
					parameterResources.resize(arrayEntryTypeIndex+sizeof(CgParameterStructure));
					parameterArrayEntry.typeIndex = CNV2END(arrayEntryTypeIndex);

					CgParameterStructure *parameterStructure = (CgParameterStructure*)(&parameterResources[arrayEntryTypeIndex]);
					parameterStructure->memberCount = CNV2END((unsigned short)structureType->_elements.size());
					parameterStructure->reserved = CNV2END((unsigned short)0);

					if (hasDefaults)
						fillStructureItems(containers,structureType,endianness,parameterEntries,parameterResources,stringTable,&_arrayResourceIndex,&_arrayDefaultValueIndex);
					else
						fillStructureItems(containers,structureType,endianness,parameterEntries,parameterResources,stringTable,&_arrayResourceIndex);
				}

				if (hasDefaults)
				{
					//should check this value, but it's not really easy, need the entire size of the complex structure in registers
					//unsigned short ArrayBaseTypeCount = _arrayDefaultValueIndex -containers._defaultValuesIndices[currentDefaultIndex*2+1];
					currentDefaultIndex++;
				}
				//containers._defaultValuesIndices[currentDefaultIndex*2] = CNV2END((unsigned short)(parameterEntries.size() - 1));
				//currentDefaultIndex++;


				//default value expansion: we have an array of struct: we have one default value index for the array
				//since the array is unrolled we don't give any resource to the array, but instead we give the resource to each item of each structure
				//those items can be arrays or structure themselves

				//we have already output the content of the array continue to the next structure item
				continue;
			}
			else
			{
				//create the type item following the array , the type section will fill typeindex value
				size_t size = parameterEntries.size();
				parameterEntries.resize(size+1);
				parameterEntry = &parameterEntries[size];
				parameterEntry->nameOffset = CNV2END(0);
				parameterEntry->flags = CNV2END(structuralElement->_flags);

				//prepare a typeIndex for this type
				typeIndex = ((unsigned short)parameterResources.size());
				JS_ASSERT2((typeIndex&3) == 0,("typeindex must be aligned on 4 bytes"));
				parameterEntry->typeIndex = CNV2END(typeIndex);

				//keep the parameterEntryIndex to assign the default values to the proper item
				parameterEntryIndex = (int)size - 1;
			}
		}
		else
		{
			unsigned short contiguousFlag = CGP_CONTIGUOUS;
			parameterEntry->flags |= CNV2END(contiguousFlag);
			size_t size = parameterEntries.size();
			parameterEntryIndex = (int)size - 1;
		}

		if (itemType->_type<128)
		{
			parameterResources.resize(typeIndex+sizeof(CgParameterResource));
			CgParameterResource *parameterResource = (CgParameterResource*)(&parameterResources[typeIndex]);

			if (itemType->_type + CG_TYPE_START_ENUM == CG_BOOL) // checking _resource == CG_B only works if referenced
			    {
				parameterResource->type = CNV2END((unsigned short)CGP_SCF_BOOL);
			    }
			else
			    {
				parameterResource->type = CNV2END((unsigned short)(itemType->_type + CG_TYPE_START_ENUM));
			    }

			if ((structuralElement->_flags & CGPV_MASK) == CGPV_UNIFORM || (structuralElement->_flags & CGPV_MASK) == CGPV_CONSTANT)
			{
				//use the resource index of the array if we are in an array: pbl with arrays containing samplers...
				if (itemType->_type +CG_TYPE_START_ENUM >= CG_SAMPLER1D && itemType->_type +CG_TYPE_START_ENUM<= CG_SAMPLERCUBE)
					parameterResource->resource = CNV2END(_resource); //sampler we need the texture unit
				else
				{
					if (arrayResourceIndex)
					{
						unsigned short tmp = *arrayResourceIndex;
						unsigned short localflags = CNV2END(parameterEntry->flags);
						if (!bIsVertexProgram)
						{
							parameterResource->resource = CNV2END(tmp);
							int embeddedConstantCount = CNV2END(containers._resources[tmp+1]);
							(*arrayResourceIndex) = tmp+1+1+embeddedConstantCount;
							if (embeddedConstantCount == 0 && (CNV2END(containers._resources[tmp]) == 0))
							{
								//we are unrolling an array of struct, so we get individual items
								//we should mark if they are referenced individually
								if (parameterResource->resource == 0xffff)
									localflags &= ~CGPF_REFERENCED;
							}
						}
						else
						{
							//if it's not contiguous , I am not sure what to do
							JS_ASSERT2(CNV2END(parameterEntry->flags) & CGP_CONTIGUOUS,("assumed parameterEntry->flags & CGP_CONTIGUOUS"));

							if (structuralElement->_flags & CGPF_SHARED)
							{
								int stride = getStride(itemType);
								parameterResource->resource = *arrayResourceIndex;
								(*arrayResourceIndex) = tmp+stride;
							}
							else
							{
								//this is a  hack, the info will be at 2 places
								parameterResource->resource = containers._resources[tmp];//_resources is already converted
								(*arrayResourceIndex) = tmp+1;
							}

							//we are unrolling an array of struct, so we get individual items
							//we should mark if they are referenced individually
							if (parameterResource->resource == 0xffff)
								localflags &= ~CGPF_REFERENCED;
						}
						//update the flags
						parameterEntry->flags = CNV2END(localflags);
					}
					else
						parameterResource->resource = CNV2END(_resourceIndex); //uniform not a sampler we need the register
				}
			}
			else
			{
				JS_ASSERT2((structuralElement->_flags & CGPV_MASK) == CGPV_VARYING,("assumed varying"));
				JS_ASSERT2(arrayResourceIndex == NULL,("varying within array of structures not yet supported"));
				parameterResource->resource = CNV2END(itemType->_resource); //we need the bind location for the varying
			}

			if (containers._defaultValuesIndices.size())
			{
				if (structure->_root)
				{
					//TODO: what about default values on structures at the root level, the structure will flatten out so the default should go on the individual items
					//but they are not at root so the indices cannot be represented right now I guess.
					//semantics and default values indices need to be updated:
					//we are trying to find the index in the sorted array of default value indices, if we are already at the end of the array we stop here
					if (currentDefaultIndex < (int)(containers._defaultValuesIndices.size()/2) && containers._defaultValuesIndices[currentDefaultIndex*2] == CNV2END((unsigned short)i))
					{
						containers._elfDefaultsIndices.push_back(CNV2END((unsigned short)(parameterEntryIndex)));
						containers._elfDefaultsIndices.push_back(containers._defaultValuesIndices[currentDefaultIndex*2+1]);
						//containers._defaultValuesIndices[currentDefaultIndex*2] = CNV2END((unsigned short)(parameterEntries.size() - 1));
						currentDefaultIndex++;
					}
				}
				else if (arrayDefaultValueIndex)
				{
					//we are in a struct or in an array of struct which has a top level default, each time we have a basic type we need to increase
					//the index

					containers._elfDefaultsIndices.push_back(CNV2END((unsigned short)(parameterEntryIndex)));
					containers._elfDefaultsIndices.push_back(*arrayDefaultValueIndex);
					//increment the default values index by the number of defaultValues slot used by this type

					int typeRegisterCount = getStride(itemType);
					*arrayDefaultValueIndex = CNV2END( (unsigned short)((CNV2END((*arrayDefaultValueIndex)))+typeRegisterCount*4));

					//no handling of the currentDefaultIndex, which is only valid at root.
				}
			}
		}
		else if (itemType->_type == CG_STRUCT + 128)
		{
			JS_ASSERT2(arrayResourceIndex == NULL,("struct within array of structures not yet supported"));
			unsigned short structureFlag  = CGP_STRUCTURE;
			parameterEntry->flags |= CNV2END(structureFlag);

			CgStructureType *structureType = (CgStructureType*)itemType;
			parameterResources.resize(typeIndex+sizeof(CgParameterStructure));
			CgParameterStructure *parameterStructure = (CgParameterStructure*)(&parameterResources[typeIndex]);
			parameterStructure->memberCount = CNV2END((unsigned short)structureType->_elements.size());
			parameterStructure->reserved = CNV2END((unsigned short)0);

			fillStructureItems(containers,structureType,endianness,parameterEntries,parameterResources,stringTable);

			if (containers._defaultValuesIndices.size() && structure->_root)
			{
				if (currentDefaultIndex < (int)(containers._defaultValuesIndices.size()/2) && containers._defaultValuesIndices[currentDefaultIndex*2] == CNV2END((unsigned short)i))
				{
						containers._elfDefaultsIndices.push_back(CNV2END((unsigned short)(parameterEntryIndex)));
						containers._elfDefaultsIndices.push_back(containers._defaultValuesIndices[currentDefaultIndex*2+1]);
						currentDefaultIndex++;
				}
			}
		}
	}
	JS_ASSERT2(!structure->_root || currentDefaultIndex == (int)containers._defaultValuesIndices.size()/2,("error\n"));
}


static int getStride(CgBaseType *type)
{
	if (type->_type <128)
	{
		if (classnames[type->_type-1] == CG_PARAMETERCLASS_MATRIX)
			return rows[type->_type-1];
		else
			return 1;
	}
	else
	{
		if (type->_type == CG_STRUCT + 128)
		{
			CgStructureType *structureType = (CgStructureType *)type;
			int res = 0;
			int i;
			int count = (int)structureType->_elements.size();
			for (i=0;i<count;i++)
				res += getStride(structureType->_elements[i]._type);
			return res;
		}
		else
		{
			JS_ASSERT2(0,("arrays of arrays not supported"));
			return -9999999;
		}
		//if array ??
	}
}

static int getSizeofSubArray(_CGNVCONTAINERS &containers, int dimensionIndex, int dimensionCount, int endianness)
{
	unsigned char elfEndianness = endianness; //used in the macro CNVEND
	int res = 1;
	int i;
	for (i=0;i<dimensionCount;i++)
	{
		res *= (int)CNV2END(containers._dimensions[dimensionIndex + i]);
	}
	return res;
}

/*static unsigned int constTableAddUnique( float *value )//128 bytes
{
unsigned int constTableSize = (int)_constTable.size();
unsigned int i;
for (i=0;i<constTableSize;i++)
{
if (!memcmp(value,(char*)(&_constTable[0])+i,sizeof(_float4)))
{
return i;
}
}
//not found add it:
_constTable.push_back(*(_float4*)value);

return constTableSize;
}*/

template<class Type> static size_t array_size(STL_NAMESPACE vector<Type> &array)
{
	return (unsigned int)array.size()*sizeof(array[0]);
}

template<class Type> static void array_push(char* &parameterOffset, STL_NAMESPACE vector<Type> &array)
{
	size_t dataSize = array.size()*sizeof(array[0]);
	memcpy(parameterOffset,&array[0],dataSize);
	parameterOffset += dataSize;
}

Elf32_Word inline static swap16(const Elf32_Word v)
{
	return (v>>16) | (v<<16);
}

unsigned short getFlags(CGenum var, CGenum dir, int no,	bool is_referenced, bool is_shared, int paramIndex)
{
	//the following is error prone, use 2 enum array to do the matching instead
	//variability
	Elf32_Half flags = 0;
	if (var == CG_VARYING)
		flags |= CGPV_VARYING;
	else if (var == CG_UNIFORM)
		flags |= CGPV_UNIFORM;
	else if (var == CG_CONSTANT)
		flags |= CGPV_CONSTANT;
	else if (var == CG_MIXED)
		flags |= CGPV_MIXED;

	//direction
	if (dir == CG_IN)
		flags |= CGPD_IN;
	else if (dir == CG_OUT)
		flags |= CGPD_OUT;
	else if (dir == CG_INOUT)
		flags |= CGPD_INOUT;

	//boolean
	if (is_referenced)
		flags |= CGPF_REFERENCED;
	if (is_shared)
		flags |= CGPF_SHARED;

	//is it a global parameter ?
	if (no == -1)
		flags |= CGPF_GLOBAL;
	else if (no == -2)
		flags |= CGP_INTERNAL;
	else
	{
		JS_ASSERT2(no>=0,("error\n"));
		/*if ( no == paramIndex ) //means that a previous parameter already had the same param index, it's an aggregated type
		flags |= CGP_AGGREGATED;*/
		paramIndex = no;
	}
	return flags;
}

//////////////////////////////////////////////////////////////////////////
// Debug print

#ifdef JS_CG_CONVERTER_REPORT
void debugPrintElementID(const char *name, unsigned short flag, CgBaseType *type)
{
	printf("%s",name);
	setrgb(2);
	printf("  (%s)",typenames[type->_type-1]);
	setrgb(4);
	//print the flag

	//direction
	unsigned int direction = flag & CGPD_MASK;
	if (direction == CGPD_IN)
		printf(" in");
	else if (direction == CGPD_OUT)
		printf(" out");
	else if (direction == CGPD_INOUT)
		printf(" inout");
	//variability
	unsigned int variability = flag & CGPV_MASK;
	if ( variability == CGPV_VARYING)
		printf(" varying");
	else if (variability == CGPV_UNIFORM)
		printf(" uniform");
	else if (variability == CGPV_CONSTANT)
		printf(" constant");
	else if (variability == CGPV_MIXED)
		printf(" mixed");

	//global or local parameter:
	if (flag & CGPF_GLOBAL)
		printf(" global");
	else if (flag & CGP_INTERNAL)
		printf(" internal");
	/*else if (flag & CGP_AGGREGATED)
	printf(" aggregated");*/

	if (flag & CGPF_REFERENCED)
		printf(" ref");
	if (flag & CGPF_SHARED)
		printf(" shared");
	setrgb(1);
	printf(" %s",bindlocationsNames[type->_resource]);
}

void debugPrintStructure(_CGNVCONTAINERS &containers, CgStructureType *structure, int endianness, STL_NAMESPACE vector<float> &defaultValues, int indent)
{
	unsigned char elfEndianness = endianness; //used in the macro CNVEND
	int i;

	int count = (int)structure->_elements.size();
	for (i=0;i<count;i++)
	{
		int k;
		CgStructureType::CgStructuralElement *element = &structure->_elements[i];
		int typeRegisterCount = 0;
		//indentation
		for (k=0;k<indent;k++) printf(" ");
		if (element->_type->_type <128)
		{
			debugPrintElementID(element->_name,element->_flags,element->_type);
			setrgb(4);
			if (!structure->_insideArray)
			{
				printf(" res:");
				if (classnames[element->_type->_type-1] == CG_PARAMETERCLASS_MATRIX)
				{
					typeRegisterCount = rows[element->_type->_type-1];
					if (bIsVertexProgram)
					{
						for (k=0;k<rows[element->_type->_type-1];k++)
						{
							printf(" %i",element->_type->_resourceIndex + k); //matrices are contiguous in a vp
						}
					}
					else
					{
						int index = element->_type->_resourceIndex;
						for (k=0;k<rows[element->_type->_type-1];k++)
						{
							printf(" %i",CNV2END(containers._resources[index++]));
							int embeddedConstantCount = CNV2END(containers._resources[index++]);
							if (embeddedConstantCount)
							{
								printf("(");
								int l;
								for (l=0;l<embeddedConstantCount;l++)
								{
									if (l) printf(",");
									printf("%i",CNV2END(containers._resources[index++]));
								}
								printf(")");
							}
						}
					}
				}
				else
				{
					typeRegisterCount = 1;
					if (bIsVertexProgram)
					{
						printf(" %i",element->_type->_resourceIndex);
					}
					else
					{
						int index = element->_type->_resourceIndex;
						printf(" %i",CNV2END(containers._resources[index++]));
						int embeddedConstantCount = CNV2END(containers._resources[index++]);
						if (embeddedConstantCount)
						{
							printf("(");
							int l;
							for (l=0;l<embeddedConstantCount;l++)
							{
								if (l) printf(",");
								printf("%i",CNV2END(containers._resources[index++]));
							}
							printf(")");
						}
					}
				}
			}
			setrgb(0);
			printf("\n");
		}
		else if (element->_type->_type == CG_ARRAY+128)
		{
			CgArrayType *arrayType = (CgArrayType *)element->_type;
			int itemCounts = 1;
			char elementName[256];
			sprintf(elementName,"%s",element->_name);
			for (k=0;k<arrayType->_dimensionCount;k++)
			{
				int dim = CNV2END(containers._dimensions[arrayType->_dimensionItemCountsOffset+k]);
				char tmp[64];
				sprintf(tmp,"[%i]",dim);
				strcat(elementName,tmp);
				itemCounts *= dim;
			}
			//print first the type or the underlying structure
			if (arrayType->_elementType->_type == CG_STRUCT+128)
			{
				printf("%s\n",elementName);
				debugPrintStructure(containers,(CgStructureType*)arrayType->_elementType,endianness,defaultValues,indent+2);
			}
			else
			{
				debugPrintElementID(elementName,element->_flags,arrayType->_elementType);
				printf("\n");
			}

			setrgb(4);
			//get stride ... could be precomputed...
			int stride = getStride(arrayType->_elementType);
			typeRegisterCount = itemCounts*stride;
			for (k=0;k<indent;k++) printf(" ");
			printf("res:\n");
			if (bIsVertexProgram)
			{
				for (k=0;k<itemCounts;k++)
				{
					int l;
					for (l=0;l<indent+4;l++) printf(" ");
					for (l=0;l<stride;l++)
					{
						////special case here for shared vertex array ??? I consider them contiguous and referenced
						//do we have a shared contiguous
						bool sharedContiguous = (element->_flags & CGPF_SHARED) && (element->_flags & CGP_CONTIGUOUS);
						if (sharedContiguous)
							printf(" %i",arrayType->_resourceIndex + k*stride + l);
						else
							printf(" %i",CNV2END(containers._resources[arrayType->_resourceIndex + k*stride + l]));
					}
					printf("\n");
				}
			}
			else
			{
				int index = arrayType->_resourceIndex;
				for (k=0;k<itemCounts;k++)
				{
					int l;
					for (l=0;l<indent+4;l++) printf(" ");
					for (l=0;l<stride;l++)
					{
						printf(" %i",CNV2END(containers._resources[index++]));
						int embeddedConstantCount = CNV2END(containers._resources[index++]);
						if (embeddedConstantCount)
						{
							printf("(");
							for (l=0;l<embeddedConstantCount;l++)
							{
								if (l) printf(",");
								printf("%i",CNV2END(containers._resources[index++]));
							}
							printf(")");
						}
					}
					printf("\n");
				}
			}
			setrgb(0);
		}
		else if (element->_type->_type == CG_STRUCT+128)
		{
			printf("%s\n",element->_name);
			debugPrintStructure(containers,(CgStructureType*)element->_type,endianness,defaultValues,indent+2);
		}

		//check for defaults
		if (structure->_root)
		{
			int defaultCount = (int)containers._defaultValuesIndices.size()/2;
			int j;
			for (j=0;j<defaultCount;j++)
			{
				if (containers._defaultValuesIndices[j*2] == CNV2END((unsigned short)i))
				{
					setrgb(3);
					printf("def:\n");
					unsigned short defaultValuesIndex = CNV2END(containers._defaultValuesIndices[j*2+1]);
					int k;
					for (k=0;k<typeRegisterCount;k++)
					{
						unsigned int v0 = CNV2END(*(unsigned int*)&defaultValues[defaultValuesIndex+k*4+0]);
						unsigned int v1 = CNV2END(*(unsigned int*)&defaultValues[defaultValuesIndex+k*4+1]);
						unsigned int v2 = CNV2END(*(unsigned int*)&defaultValues[defaultValuesIndex+k*4+2]);
						unsigned int v3 = CNV2END(*(unsigned int*)&defaultValues[defaultValuesIndex+k*4+3]);
						printf("%g %g %g %g\n",*(float*)&v0,*(float*)&v1,*(float*)&v2,*(float*)&v3);
					}
					setrgb(0);
				}
			}
		}
	}
}

#endif //#ifdef JS_CG_CONVERTER_REPORT
