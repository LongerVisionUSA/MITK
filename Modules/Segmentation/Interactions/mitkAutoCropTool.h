/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef mitkAutoCropTool_h_Included
#define mitkAutoCropTool_h_Included

#include "mitkCommon.h"
#include "mitkSegmentationsProcessingTool.h"
#include <MitkSegmentationExports.h>

namespace mitk
{
  /**
    \brief Crops selected segmentations.

    \ingroup ToolManagerEtAl
    \sa mitk::Tool
    \sa QmitkInteractiveSegmentation

    Last contributor: $Author$
  */
  class MITKSEGMENTATION_EXPORT AutoCropTool : public SegmentationsProcessingTool
  {
  public:
    mitkClassMacro(AutoCropTool, SegmentationsProcessingTool);
    itkFactorylessNewMacro(Self) itkCloneMacro(Self)

      const char **GetXPM() const override;
    const char *GetName() const override;

  protected:
    bool ProcessOneWorkingData(DataNode *node) override;
    std::string GetErrorMessage() override;

    AutoCropTool(); // purposely hidden
    ~AutoCropTool() override;
  };

} // namespace

#endif
