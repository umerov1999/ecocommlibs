/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef _FILE_XMLWRITER_H_
#define _FILE_XMLWRITER_H_

#include <common/io/outputstream.h>

#ifdef __cplusplus
extern "C"
{
#endif


struct XmlWriter;

struct XmlWriter *xmlwCreate(struct OutputStream *iptOutput);

void xmlwDestroy(struct XmlWriter **iopptXmlWriter);

s32 xmlwStartDocument(struct XmlWriter *iptXmlWriter);

void xmlwEndDocument(struct XmlWriter *iptXmlWriter);

s32 xmlwAddElement(struct XmlWriter *iptXmlWriter, char *izTag);

s32 xmlwEndElement(struct XmlWriter *iptXmlWriter);

s32 xmlwAddAttribute(struct XmlWriter *iptXmlWriter, char *izKey, char *izValue);

s32 xmlwAddText(struct XmlWriter *iptXmlWriter, char *izText);

s32 xmlwAddComment(struct XmlWriter *iptXmlWriter, const char *izComment);


#ifdef __cplusplus
}
#endif


#endif /* _FILE_XMLWRITER_H_ */
