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

#ifndef MITKUSTelemedDevice_H_HEADER_INCLUDED_
#define MITKUSTelemedDevice_H_HEADER_INCLUDED_

#include "mitkUSDevice.h"
#include "mitkUSTelemedImageSource.h"
#include "mitkUSTelemedScanConverterPlugin.h"
#include "mitkUSTelemedProbesControls.h"
#include "mitkUSTelemedBModeControls.h"
#include "mitkUSTelemedDopplerControls.h"

#include "mitkUSTelemedSDKHeader.h"

namespace mitk {
  /**
    * \brief Implementation of mitk::USDevice for Telemed API devices.
    * Connects to a Telemed API device through its COM library interface.
    *
    * This class handles all API communications and creates interfaces for
    * b mode, doppler and probes controls.
    * Images given by the device are put into an object of
    * mitk::USTelemedImageSource.
    */
  class USTelemedDevice : public USDevice
  {
  public:
    mitkClassMacro(USTelemedDevice, mitk::USDevice);
    mitkNewMacro2Param(Self, std::string, std::string);

    /**
    * \brief Returns the class of the device.
    */
    virtual std::string GetDeviceClass();

    virtual USControlInterfaceBMode::Pointer GetControlInterfaceBMode();
    virtual USControlInterfaceProbes::Pointer GetControlInterfaceProbes();
    virtual USControlInterfaceDoppler::Pointer GetControlInterfaceDoppler();

    /**
      * \brief Is called during the initialization process.
      * There is nothing done on the initialization of a mik::USTelemedDevive object.
      *
      * \return always true
      */
    virtual bool OnInitialization();

    /**
      * \brief Is called during the connection process.
      * Connect to the Telemed API and try to get available probes from the device.
      *
      * \return true if successfull, false if no device is connected to the pc
      * \throws mitk::Exception if something goes wrong at the API calls
      */
    virtual bool OnConnection();

    /**
      * \brief Is called during the disconnection process.
      * Deactivate and remove all Telemed API controls. A disconnect from the
      * Telemed API is not possible for which reason the hardware stays in connected
      * state even after calling this method.
      *
      * \return always true
      * \throws mitk::Exception if something goes wrong at the API calls
      */
    virtual bool OnDisconnection();

    /**
      * \brief Is called during the activation process.
      * After this method is finished, the device is generating images in b mode.
      * Changing scanning mode is possible afterwards by using the appropriate
      * control interfaces.
      *
      * \return always true
      * \throws mitk::Exception if something goes wrong at the API calls
      */
    virtual bool OnActivation();

    /**
      * \brief Is called during the deactivation process.
      * After a call to this method the device is connected, but not producing images anymore.
      *
      * \return always true
      * \throws mitk::Exception if something goes wrong at the API calls
      */
    virtual bool OnDeactivation();

    /**
      * \brief Changes scan state of the device if freeze is toggeled in mitk::USDevice.
      */
    virtual void OnFreeze(bool freeze);

    /**
      * \brief Grabs the next frame from the Video input.
      * This method is called internally, whenever Update() is invoked by an Output.
      */
    void GenerateData();

    /**
      * \brief Getter for main Telemed API object.
      * This method is for being called by Telemed control interfaces.
      */
    IUsgfw2* GetUsgMainInterface();

    /**
      * \brief Changes active IUsgDataView of the device.
      * This method is for being called by Telemed control interfaces.
      */
    void SetActiveDataView(IUsgDataView*);

  protected:
    /**
      * Constructs a mitk::USTelemedDevice object by given manufacturer
      * and model string. These strings are just for labeling the device
      * in the micro service.
      *
      * Control interfaces and image source are available directly after
      * construction. Registration at the micro service happens not before
      * initialization method was called.
      */
    USTelemedDevice(std::string manufacturer, std::string model);
    virtual ~USTelemedDevice();

    USImageSource::Pointer GetUSImageSource( );

    void ReleaseUsgControls( );

    /**
      * \brief Stop ultrasound scanning by Telemed API call.
      *
      * \throw mitk::Exception if API call returned with an error
      */
    void StopScanning( );

    USTelemedProbesControls::Pointer    m_ControlsProbes;
    USTelemedBModeControls::Pointer     m_ControlsBMode;
    USTelemedDopplerControls::Pointer   m_ControlsDoppler;

    USTelemedImageSource::Pointer       m_ImageSource;

    IUsgfw2*                            m_UsgMainInterface;
    IProbe*                             m_Probe;
    IUsgDataView*                       m_UsgDataView;
    IUsgCollection*                     m_ProbesCollection;
  };
} // namespace mitk

#endif // MITKUSTelemedDevice_H_HEADER_INCLUDED_