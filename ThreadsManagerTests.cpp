#include "ThreadsManagerTests.h"


const char* poolToStr(ePoolType pt)
{
    switch(pt)
    {
        case PT_LAZYCACHED: return "lazy cached";break;
        case PT_AUTOCNT: return "auto cnted";break;
        case PT_PRECACHED: return "preacached";break;
        default: return "unknown";break;
    }
}

thread_holder_t* createObjectTest(unsigned int& pId,ePoolType pt, bool expectAlive=true)
{


    const char* debugStr = poolToStr(pt);
    eManagerError err;
    thread_holder_t* ptr=ThreadsManagerTest::GetInstance().getObject(pId,err,pt);

    if (ptr==NULL)
    {
        printf("%s %s object failed to be created with error %d\n",expectAlive ? "[FAILED]" : "[SUCCESS]",debugStr,err);
    } else {
        printf("%s %s object created with ID:%d\n",!expectAlive ? "[FAILED]" : "[SUCCESS]",debugStr,pId);

    }
    return ptr;
}

void destroyPoolTest(unsigned int refId, ePoolType pt)
{
    const char* debugStr =poolToStr(pt);

    ThreadsManagerTest::GetInstance().destroyPool(pt);
    thread_holder_t* ptr = ThreadsManagerTest::GetInstance().getObjectForId(refId);



    if (ptr!=NULL)
    {
        printf("[FAILED] Pool destroy: Id %d for %s object not null\n",refId,debugStr);
    } else {
        printf("[SUCCESS] pool destroy null retrieved for ID %d %s object\n",refId,debugStr);
    }
}

void destroyObjectTest(unsigned int refId, ePoolType pt, bool expectAlive=false, bool doubleRelease=false)
{
    const char* debugStr = poolToStr(pt);

    ThreadsManagerTest::GetInstance().releaseObject(refId);
    thread_holder_t* ptr = ThreadsManagerTest::GetInstance().getObjectForId(refId);

    if (ptr==NULL)
    {
        printf("%s Id %d for %s object is null after release\n", expectAlive ? "[FAILED]" : "[SUCCESS]", refId,debugStr);
    } else {
        printf("%s Id %d for %s object not null after release\n", !expectAlive ? "[FAILED]" : "[SUCCESS]", refId, debugStr);
        if (doubleRelease)
        {
            ThreadsManagerTest::GetInstance().releaseObject(refId);
        }
    }
}

void getObjTest(unsigned int refId, bool expectAlive=true, bool destroyAfterGet=false)
{
    thread_holder_t* ptr = ThreadsManagerTest::GetInstance().getObjectForId(refId);

    if (ptr==NULL)
    {
        printf("%s Id %d null after get\n", expectAlive ? "[FAILED]" : "[SUCCESS]", refId);
    } else {
        printf("%s Id %d not null after get\n", expectAlive ? "[SUCCESS]" : "[FAILED]", refId);
        if (destroyAfterGet)
        {
            ThreadsManagerTest::GetInstance().releaseObject(refId);
        }
    }
}

void TestThreadsManager()
{

    /*
    ThreadsManagerTest* ptr = new ThreadsManagerTest();

    ptr->createPrecachePool(1);
    ptr->destroyAll();

    delete ptr;*/

    unsigned int id1=0;
    unsigned int id2=0;
    unsigned int id3=0;

    // precache test

    ThreadsManagerTest::GetInstance().createPrecachePool(5);


    createObjectTest(id1,PT_PRECACHED);
    id2 = 4;
    getObjTest(id2);
    id3 = 5;
    getObjTest(id3,false);
    createObjectTest(id3,PT_PRECACHED,false);
    destroyObjectTest(id2,PT_PRECACHED,true);

    destroyPoolTest(id1,PT_PRECACHED);

    // lazy test

    id1 = -1;
    id2 = -1;
    id3 = -1;


    createObjectTest(id1,PT_LAZYCACHED);
    createObjectTest(id1,PT_LAZYCACHED);
    getObjTest(id1);

    getObjTest(id2,false);
    createObjectTest(id2,PT_LAZYCACHED);
    getObjTest(id2);
    destroyObjectTest(id2,PT_LAZYCACHED,true);

    destroyPoolTest(id2,PT_LAZYCACHED);


    getObjTest(id3,false);
    createObjectTest(id3,PT_LAZYCACHED);
    destroyObjectTest(id3,PT_LAZYCACHED,true);

    destroyPoolTest(id3,PT_LAZYCACHED);

    // auto test

    id1 =-1;
    id2 =-1;
    id3 =-1;


    createObjectTest(id2,PT_AUTOCNT,true);
    getObjTest(id2,true,true);
    destroyObjectTest(id2,PT_AUTOCNT,false);
    getObjTest(id2,false,true);


    createObjectTest(id1,PT_AUTOCNT);
    createObjectTest(id1,PT_AUTOCNT);
    createObjectTest(id1,PT_AUTOCNT);
    destroyObjectTest(id1,PT_AUTOCNT,true,true);
    destroyObjectTest(id1,PT_AUTOCNT,true,true);
    getObjTest(id1,true,true);


    destroyObjectTest(id1,PT_AUTOCNT);

    createObjectTest(id3,PT_AUTOCNT);
    createObjectTest(id3,PT_AUTOCNT);
    getObjTest(id3,true);

    destroyPoolTest(id3,PT_AUTOCNT);

    ThreadsManagerTest::GetInstance().destroyAll();

}



