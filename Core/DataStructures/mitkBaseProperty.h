#ifndef BASEPROPERTY_H_HEADER_INCLUDED_C1F4DF54
#define BASEPROPERTY_H_HEADER_INCLUDED_C1F4DF54

#include "mitkCommon.h"

namespace mitk {

//##ModelId=3D6A0ED703CF
class BaseProperty : public itk::Object
{
public:

	mitkClassMacro(BaseProperty,itk::Object);

protected:
    //##ModelId=3E38FF0902A7
    BaseProperty();

    //##ModelId=3E38FF0902CF
    virtual ~BaseProperty();

	virtual bool operator==(const BaseProperty& property ) const = 0;
};

} // namespace mitk



#endif /* BASEPROPERTY_H_HEADER_INCLUDED_C1F4DF54 */
