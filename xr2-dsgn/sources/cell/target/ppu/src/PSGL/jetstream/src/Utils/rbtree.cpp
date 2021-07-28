/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


#include <PSGL/rbtree.h>

namespace PSGL
{


#define SENTINEL_PRINT(s, b) if(b == &a->sentinel) sprintf(s, "sentinel"); else sprintf(s, "%ld", (long) b)

#define DEBUGDUMP 0

#if DEBUGDUMP

static void internal_dump(FILE *f, RBTree *a, RBTree::RBNode *r)
{
    char node[32], left[32], right[32], up[32], mColor[32];

    if(r == &a->sentinel)
    {
        return;
    }

    SENTINEL_PRINT(node, r);
    SENTINEL_PRINT(left, r->mLeft);
    SENTINEL_PRINT(right, r->mRight);
    SENTINEL_PRINT(up, r->mUp);
    sprintf(mColor, "%s", (r->mColor == RBNode::Red ? "red" : "black"));

    fprintf(f, "\t%s [label=%d, mColor=%s]\n", node, *((uint32_t *) r->mKey), mColor); 
    fprintf(f, "\t%s -> %s\n\t%s -> %s\n", node, left, node, right);
    internal_dump(f, a, r->mLeft);
    internal_dump(f, a, r->mRight);
}

#endif // DEBUGDUMP

//graphviz

void RBTree::dumpSubtree(RBNode *r)
{
#if DEBUGDUMP
    static int x = 0;
    char buf[256];
    FILE *f;
    
    sprintf(buf, "treedump.%d", x++);
    f = fopen(buf, "w");
    
    fprintf(f, "digraph RBTree {\n");
    internal_dump(f, this, r);
    fprintf(f, "}\n");
    
    fclose(f);
#endif // DEBUGDUMP
}


RBTree::RBNode::RBNode()
{
    mValue = 0;
    mKey = 0;
    mLeft = 0;
    mRight = 0;
    mUp = 0;
    mColor = Black;
}    

RBTree::RBTree(Cmpfunc compare)
{
    mCompare = compare;
    mRoot = &mSentinel;
    mSentinel.mColor = RBNode::Black;
    mNelem = 0;
}


  

RBTree::~RBTree()
{
    clear();
}




RBTree::RBNode *RBTree::search(const void *key) const
{
  register RBNode *r = 0;
  register const RBNode *sentinel = &mSentinel;
  
  
  r = mRoot;
  while(r != sentinel)
  {
      if(mCompare(key, r->mKey))
      {
          r = r->mLeft;
      }
      else if(mCompare(r->mKey, key))
      {
          r = r->mRight;
      }
      else
      {
          break;   
      }
  }

  if(r == sentinel)
  {
    r = 0;
  }
    
  return r;
}



void RBTree::leftRotate(RBNode *x)
{
  RBNode *y = 0, *sentinel=&mSentinel;
  y = x->mRight;
  x->mRight = y->mLeft;
  if(y->mLeft != sentinel)
  {
    y->mLeft->mUp = x;
  }
  y->mUp = x->mUp;
  if(x->mUp == sentinel)
  {
    mRoot = y;
  } else {
    if(x == x->mUp->mLeft)
    {
      x->mUp->mLeft = y;
    }
    else
      x->mUp->mRight = y;
  }
  y->mLeft = x;
  x->mUp = y;
}

void RBTree::rightRotate(RBNode *y)
{
  register RBNode *x = 0, *sentinel=&mSentinel;
  x = y->mLeft;
  y->mLeft = x->mRight;
  if(x->mRight != sentinel)
  {
    x->mRight->mUp = y;
  }
  x->mUp = y->mUp;
  if(y->mUp == sentinel)
  {
    mRoot = x;
  } else {
    if(y == y->mUp->mRight)
    {
      y->mUp->mRight = x;
    }
    else
      y->mUp->mLeft = x;
  }
  x->mRight = y;
  y->mUp = x;
}


void RBTree::insertFix(RBNode *z)
{
  RBNode *y = 0;

  while(z->mUp->mColor == RBNode::Red)
  {
    if(z->mUp == z->mUp->mUp->mLeft)
    {
      y = z->mUp->mUp->mRight;
      if(y->mColor == RBNode::Red)
      {
        y->mColor = RBNode::Black;      
        z->mUp->mColor = RBNode::Black;
        z->mUp->mUp->mColor = RBNode::Red;
        z = z->mUp->mUp;
      } else {
        if(z == z->mUp->mRight)
        {
          z = z->mUp;
          leftRotate(z);
        }
        z->mUp->mColor = RBNode::Black;
        z->mUp->mUp->mColor = RBNode::Red;
        rightRotate(z->mUp->mUp);
      }
    } else {
      y = z->mUp->mUp->mLeft;
      if(y->mColor == RBNode::Red)
      {
        z->mUp->mColor=RBNode::Black;
        y->mColor = RBNode::Black;
        z->mUp->mUp->mColor = RBNode::Red;
        z = z->mUp->mUp;
      } else {
        if(z == z->mUp->mLeft)
        {
          z = z->mUp;
          rightRotate(z);
        }
        z->mUp->mColor = RBNode::Black;
        z->mUp->mUp->mColor = RBNode::Red;
        leftRotate(z->mUp->mUp);
      }
    }
  }

  mRoot->mColor = RBNode::Black;
}


void RBTree::insert(const void *key, void *value)
{
  RBNode *sentinel = &mSentinel;
  RBNode *x = 0, *y = 0, *z = 0;
  
  z = new RBNode;
  
  z->mKey = key;
  z->mValue = value;
  

  y = sentinel;
  x = mRoot;

  while(x != sentinel)
  {
    y = x;
        
    if(mCompare(key, x->mKey))
    {
      x = x->mLeft;
    } else {
      x = x->mRight;
    }
  }

  z->mUp = y;

  if(y == sentinel)
  {
    mRoot = z;
  } else {
        if(mCompare(key, y->mKey))
        {
            y->mLeft = z;
        } else {
            y->mRight = z;
        }  
  }

  z->mLeft = sentinel;
  z->mRight = sentinel;
  z->mColor = RBNode::Red;
  insertFix(z);
  mNelem++;

}


void RBTree::deleteFix(RBNode *x)
{
  register RBNode *w = 0;
  
  while(x != mRoot && x->mColor == RBNode::Black)
  {
    if(x == x->mUp->mLeft)
    {
      w = x->mUp->mRight;
      if(w->mColor == RBNode::Red)
      {
        w->mColor = RBNode::Black;
        x->mUp->mColor = RBNode::Red;
        leftRotate(x->mUp);
        w = x->mUp->mRight;
      }
      if(w->mLeft->mColor == RBNode::Black && w->mRight->mColor == RBNode::Black)
      {
        w->mColor = RBNode::Red;
        x = x->mUp;
      } else {
        if(w->mRight->mColor == RBNode::Black)
        {
          w->mLeft->mColor = RBNode::Black;
          w->mColor = RBNode::Red;
          rightRotate(w);
          w = x->mUp->mRight;
        }
        w->mColor = x->mUp->mColor;
        x->mUp->mColor = RBNode::Black;
        w->mRight->mColor = RBNode::Black;
        leftRotate(x->mUp);
        x = mRoot;
      }
    } else {
      w = x->mUp->mLeft;
      if(w->mColor == RBNode::Red)
      {
        w->mColor = RBNode::Black;
        x->mUp->mColor = RBNode::Red;
        rightRotate(x->mUp);
        w = x->mUp->mLeft;
      }
      if(w->mRight->mColor == RBNode::Black && w->mLeft->mColor == RBNode::Black)
      {
        w->mColor = RBNode::Red;
        x = x->mUp;
      } else {
        if(w->mLeft->mColor == RBNode::Black)
        {
          w->mRight->mColor = RBNode::Black;
          w->mColor = RBNode::Red;
          leftRotate(w);
          w = x->mUp->mLeft;
        }
        w->mColor = x->mUp->mColor;
        x->mUp->mColor = RBNode::Black;
        w->mLeft->mColor = RBNode::Black;
        rightRotate(x->mUp);
        x = mRoot;
      }
    }
  }
  x->mColor = RBNode::Black;
}


void RBTree::erase(const void *key)
{
  RBNode *z = (RBNode *) search(key);
  
  if(z == 0)
  {
    return;
  }
  
  erase(z);
}    

void RBTree::erase(RBNode *z)
{
  register RBNode *x = 0, *y = 0;
  register RBNode *sentinel = &mSentinel;

  if(z->mLeft == sentinel || z->mRight == sentinel)
  {
    y = z;
  } else {
    y = (RBNode *) next(z);
  }

  if(y->mLeft != sentinel)
  {
    x = y->mLeft;
  }
  else
    x = y->mRight;

  x->mUp = y->mUp;
  if(y->mUp == sentinel)
  {
    mRoot = x;
  } else {
    if(y == y->mUp->mLeft)
    {
      y->mUp->mLeft = x;
    }
    else
      y->mUp->mRight = x;
  }

  if(y != z)
  {
    z->mKey = y->mKey;
    z->mValue = y->mValue;
  }
  
  if(y->mColor == RBNode::Black)
  {
    deleteFix(x);
  }

  y->mRight = sentinel;
  y->mLeft = sentinel;
  y->mUp = sentinel;
  delete y;
  mNelem--;

}


RBTree::RBNode *RBTree::next(RBTree::RBNode *cur) const
{
  register RBNode *r = cur, *y = 0;
  register const RBNode *sentinel = &mSentinel;
  
  
  if(r == 0)
  {
    r = mRoot;
    if(r == sentinel)
    {
      goto out;
    }
    while(r->mLeft != sentinel)
      r = r->mLeft;
    goto out;
  }

  if(r->mRight != sentinel)
  {
    r = r->mRight;
    while(r->mLeft != sentinel)
      r = r->mLeft;
    goto out;
  }

  y = r->mUp;

  while(y != sentinel && r == y->mRight)
  {
    r = y;
    y = y->mUp;
  }
  r = y;
 out:
  if(r == sentinel)
  {
    r = 0;
  }
   
  return r;
}



RBTree::RBNode *RBTree::prev(RBTree::RBNode *cur) const
{
  register RBNode *r =  cur, *y = 0;
  register const RBNode *sentinel = &mSentinel;
    
  if(r == 0)
  {
    r = mRoot;
    if(r == sentinel)
    {
      goto out;
    }
    while(r->mRight != sentinel)
      r = r->mRight;
    goto out;
  }
  
  if(r->mLeft != sentinel)
  {
    r = r->mLeft;
    while(r->mRight != sentinel)
      r = r->mRight;
    goto out;
  }

  y = r->mUp;

  while(y != sentinel && r == y->mLeft)
  {
    r = y;
    y = y->mUp;
  }
  r = y;
 out:
  if(r == sentinel)
  {
    r = 0;
  }
  return  r;
}



void RBTree::clear()
{
  RBTree::RBNode *aa = 0;

  while((aa = min()) != 0)
  {
        erase(aa->mKey);
  }
  mNelem = 0;
  mRoot = &mSentinel;
}


RBTree::RBNode *RBTree::min() const
{
  register RBNode *r = mRoot;
  register const RBNode *sentinel = &mSentinel;

  if(r == sentinel)
  {
    return 0;
  }

  
  while(r->mLeft != sentinel)
    r = r->mLeft;
    
  
  return r;
}



RBTree::RBNode *RBTree::max() const
{
  register RBNode *r = mRoot;
  register const RBNode *sentinel = &mSentinel;

  if(r == sentinel)
  {
    return 0;
  }
    
  while(r->mRight != sentinel)
    r = r->mRight;
    
  return r;
}



uint32_t RBTree::size() const
{
  return mNelem;
}


//////////////////////////////////////////////////////////////////

} // namespace sce

