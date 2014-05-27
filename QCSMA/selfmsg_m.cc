//
// Generated file, do not edit! Created by opp_msgc 4.3 from selfmsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "selfmsg_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}



namespace csma {

Register_Class(SelfMsg);

SelfMsg::SelfMsg(const char *name, int kind) : cMessage(name,kind)
{
    this->src_var = 0;
    this->dest_var = 0;
    this->length_var = 0;
    this->backoff_timer_var = 0;
}

SelfMsg::SelfMsg(const SelfMsg& other) : cMessage(other)
{
    copy(other);
}

SelfMsg::~SelfMsg()
{
}

SelfMsg& SelfMsg::operator=(const SelfMsg& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void SelfMsg::copy(const SelfMsg& other)
{
    this->src_var = other.src_var;
    this->dest_var = other.dest_var;
    this->length_var = other.length_var;
    this->backoff_timer_var = other.backoff_timer_var;
}

void SelfMsg::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->src_var);
    doPacking(b,this->dest_var);
    doPacking(b,this->length_var);
    doPacking(b,this->backoff_timer_var);
}

void SelfMsg::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->src_var);
    doUnpacking(b,this->dest_var);
    doUnpacking(b,this->length_var);
    doUnpacking(b,this->backoff_timer_var);
}

int SelfMsg::getSrc() const
{
    return src_var;
}

void SelfMsg::setSrc(int src)
{
    this->src_var = src;
}

int SelfMsg::getDest() const
{
    return dest_var;
}

void SelfMsg::setDest(int dest)
{
    this->dest_var = dest;
}

double SelfMsg::getLength() const
{
    return length_var;
}

void SelfMsg::setLength(double length)
{
    this->length_var = length;
}

simtime_t SelfMsg::getBackoff_timer() const
{
    return backoff_timer_var;
}

void SelfMsg::setBackoff_timer(simtime_t backoff_timer)
{
    this->backoff_timer_var = backoff_timer;
}

class SelfMsgDescriptor : public cClassDescriptor
{
  public:
    SelfMsgDescriptor();
    virtual ~SelfMsgDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(SelfMsgDescriptor);

SelfMsgDescriptor::SelfMsgDescriptor() : cClassDescriptor("csma::SelfMsg", "cMessage")
{
}

SelfMsgDescriptor::~SelfMsgDescriptor()
{
}

bool SelfMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SelfMsg *>(obj)!=NULL;
}

const char *SelfMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SelfMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int SelfMsgDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *SelfMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "src",
        "dest",
        "length",
        "backoff_timer",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int SelfMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "src")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dest")==0) return base+1;
    if (fieldName[0]=='l' && strcmp(fieldName, "length")==0) return base+2;
    if (fieldName[0]=='b' && strcmp(fieldName, "backoff_timer")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SelfMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "double",
        "simtime_t",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *SelfMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int SelfMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SelfMsg *pp = (SelfMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SelfMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SelfMsg *pp = (SelfMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSrc());
        case 1: return long2string(pp->getDest());
        case 2: return double2string(pp->getLength());
        case 3: return double2string(pp->getBackoff_timer());
        default: return "";
    }
}

bool SelfMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SelfMsg *pp = (SelfMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setSrc(string2long(value)); return true;
        case 1: pp->setDest(string2long(value)); return true;
        case 2: pp->setLength(string2double(value)); return true;
        case 3: pp->setBackoff_timer(string2double(value)); return true;
        default: return false;
    }
}

const char *SelfMsgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *SelfMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SelfMsg *pp = (SelfMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

}; // end namespace csma

