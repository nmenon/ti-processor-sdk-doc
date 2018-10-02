﻿.. http://processors.wiki.ti.com/index.php/Processor_SDK_RTOS_SCICLIENT

Overview
--------------


Traditional Texas Instruments SoCs implement system control functions such as power management within operating systems on each of the processing units (ARM/DSP etc). However the traditional approach has had tremendous challenges to ensure system stability. Few of the challenges include:

- Complex interactions between Operating Systems on heterogeneous SoCs for generic features.
- Lack of centralized knowledge of system state.
- Complex implementation challenges when implementing workarounds for SoC errata.
- Equivalent SoC power or device management entitlement on all variations of Operating Systems.

DMSC controls the power management of the device, hence is responsible for bringing the device out of
reset, enforce clock and reset rules. DMSC power management functions are critical to bring device to low
power modes, for example DeepSleep, and sense wake-up events to bring device back online to active
state.
There is one instance of DMSC in this family of devices - WKUP_DMSC0.

Texas Instruments' System Control Interface defines the communication protocol between various processing entities to the System Control Entity on TI SoCs. This is a set of message formats and sequence of operations required to communicate and get system services processed from System Control entity in the SoC.

More information regarding the TI-SCI is given `here <http://downloads.ti.com/tisci/esd/18_08_00/>`_ .

The SCIClient is an interface to the TI-SCI protocol for RTOS and non-OS based applications. It exposes the core message details, valid module/clock IDs to the higher level software and abstracts the communication with SYSFW based on the TI-SCI protocol. These APIs can be called by power, resource and security RTOS drivers or any other non-OS or RTOS based higher level software to be able to communicate with DMSC for its services. The higher level software is expected to populate the necessary message core parameters.

The firmware which runs on DMSC is called system-firmware(SYSFW) .The documentation for SYSFW can be found at docs/system-firmware-public-documentation/ .


Terms and Abbreviation
------------------------


=====     =============================
term      definition or explanation
=====     =============================
DMSC      Device Management and Security Controller
SCI       System Control Interface
SYSFW     System Firmware
PM        Power Management
RM        Resource Management
=====     =============================


Directory Structure
--------------------

The directory structure of SCIClient is as follows:
    .. image:: ../images/sciclient_dirTree.png
                :height: 600
                :width: 900

=================================  =============================
docs/                               Contains SCIClient design documentation and SYSFW documentation.
include/                            This folder contains the files user needs to refer in order to communicate with SYSFW using `Sciclient_service`_ . The files in this folder define the request and response structures for communicating with SYSFW .
src/                                Contains source(.c) files for the driver .
examples/sciclient_firmware_boot    Contains unit tests for the driver with functions definitions in examples/sciclient_firmware_boot/sciclient_firmware_boot_tests.c"
=================================  =============================

Build and Run
----------------

This section assumes that you have PDK at PDK_INSTALL_DIR .
::
    PDK_ROOT_PATH = PDK_INSTALL_DIR/packages
    PDK_BUILD_PATH = PDK_INSTALL_DIR/packages/ti/build 
    SCICLIENT_PATH = PDK_INSTALL_DIR/drv/sciclient 


Build targets
^^^^^^^^^^^^^^^

The driver provides targets for building library, unit tests and docs .
The library and unit tests can be built from PDK_BUILD_PATH or SCICLIENT_PATH . The docs can only be built from SCICLIENT_PATH .

==========================  =============================================================================================================================  =====================
Target                      Command                                                                                                                        Description
==========================  =============================================================================================================================  =====================
Library                     make PDK_INSTALL_PATH=<your PDK_ROOT_PATH> BOARD=am65xx_evm  CORE=<mcu1_0(R5F) or mpu1_0(A53)> sciclient                       SCIClient library  
sciclient_ccs_init          make PDK_INSTALL_PATH=<your PDK_ROOT_PATH>  BOARD=am65xx_evm  CORE=<mcu1_0(R5F) or mpu1_0(A53)>  sciclient_ccs_init            SCIClient application that loads the firmware during ccs load.(No need to build this normally.)
sciclient_firmware_boot     make PDK_INSTALL_PATH=<your PDK_ROOT_PATH>  BOARD=am65xx_evm  CORE=<mcu1_0(R5F) or mpu1_0(A53)>  sciclient_firmware_boot       SCIClient example application
Design Doc                  make PDK_INSTALL_PATH=<your PDK_ROOT_PATH> designdoc                                                                           SCIClient design documentation
==========================  =============================================================================================================================  =====================

Note
:: 
     For building the example "sciclient_firmware_boot" or "your application" using SCIClient lib, make sure you have a section "boardcfg_data" defined in your linker cmd file which resides in OC-MSRAM .  

The next sections will explain how to run the example **sciclient_firmware_boot**.

Load using a boot-media
^^^^^^^^^^^^^^^^^^^^^^^^^
Follow these steps to load the **sciclient_firmware_boot** application through a boot media:

- Update the file sciclient_firmware_boot_tests.h to enable the first test **SCIClient DMSC load firmware** . Set the parameter **enableTest** as TEST_ENABLE .
Code
:: 
    /** \brief Defines the various SCICLIENT test cases. */
    App_sciclientTestParams_t gSciclientTestcaseParams[] =
    {
        {
            /** enableTest **/
            TEST_ENABLE,
    
            /** testCaseId **/
            0U,
- Refer the section `Boot-Modes <Foundational_Components.html#boot-modes>`_ for details on how to use a bootmedia.


Load using CCS
^^^^^^^^^^^^^^^^^^^^
Follow these steps to load the **sciclient_firmware_boot** application through CCS

- Update the File SCICLIENT_PATH/tools/ccsLoadDmsc/launch_am65xx.js for the following variables to your PC location .
Modify:
:: 
            //Path to GEL files
            gelFilePath = "<Your CCS Path>/ccs_base/emulation/gel/AM65xEVM/"; → GEL path location
            // Path to the directory in which this file would be residing. CCS expects
            // absolute paths to load the binaries.
            thisJsFileDirectory = "PDK_INSTALL_DIR/packages/ti/drv/sciclient/tools/ccsLoadDmsc";

- Connect the Javascript to the CCXML file with the steps shown below: (Note: this step needs to be repeated if you switch workspaces or clean your workspace).The Javascript is run on re-launch of the ccxml. If you would like to run the Javascript without relaunch then you can look ahead to the section: "Re-running the script once ccxml is already launched." Click on Debug Configurations from the button as follows:

    .. image:: ../images/sciclient_ccsLoad1.png
                :height: 300
                :width: 600
    
- Select the CCXML file from the left-hand side and populate the path to the launch_am65xx.js file in the "Initialization Script" free form field and click on "Apply".
    
    .. image:: ../images/sciclient_ccsLoad2.png
                :height: 300
                :width: 600
- Once you Launch the ccxml the java script would automatically run and connect to R5F.


.. rubric::  **Re-running the script once ccxml is already launched**

In the CCS window open the "View → Scripting Console.." 
In the CCS Scripting Console type:
:: 
    load("SCICLIENT_PATH\\tools\\ccsLoadDmsc\\launch_am65xx.js")

This would give the following output on the scripting console
:: 
    js:> load("SCICLIENT_PATH\\tools\\ccsLoadDmsc\\launch_am65xx.js")
    Connecting to DMSC_Cortex_M3_0!
    Loading DMSC Firmware...
    DMSC Firmware Load Done...
    DMSC Firmware run starting now...
    Connecting to MCU Cortex_R5_0!

API descriptions
------------------
API reference for application:
::

    #include <ti/drv/sciclient/sciclient.h>

The driver is split into 2 parts :

- `SCIClient Core`_ : This has APIs for loading SYSFW ,initialization, de-initialization and SYSFW communication .These are defined in sciclient.h .

- `SCIClient Shim Layer`_ : This thin layer abstracts the message requests and responses sent to SYSW for `Power_management`_  ,  `Resource_management`_ and `Processor_BOOT_API_Interface`_ which the user can use without worrying about the request and response structures used .

SCIClient Core
^^^^^^^^^^^^^^^^^^^^^^^

The following core APIs for SCIClient are declared in sciclient.h and include/V0/sciclient_boardcfg.h .

.. rubric:: Firmware Load
.. _Sciclient_loadFirmware:
 
This API loads SYSFW.The image must be loaded through a boot media for this API to succeed (Refer `Build and Run`_). Hence, this is typically called by SBL. Load
firmware call does not require `Sciclient_init`_ to be done before.
::
    Defined in sciclient.h as Sciclient_loadFirmware.

Typical usage
::
    static const uint32_t gSciclient_firmware[BINARY_FILE_SIZE_IN_BYTES / 4U] =SCICLIENT_FIRMWARE;
    .
    .
    .
    Sciclient_loadFirmware(gSciclient_firmware);

.. rubric:: SCIClient initialization
.. _Sciclient_init:

This API is called once for registering interrupts and creating
semaphore handles to be able to talk to the firmware.
The application should assume that the firmware is pre-loaded while
calling this API.
The firmware should have been loaded either via GEL or via the SBL 
(refer `Sciclient_loadFirmware`_ ) prior to the application calling this.

The API **Sciclient_init** takes a pointer to *Sciclient_ConfigPrms_t* as input.

-  The parameter *opModeFlag* allows 2 operation modes for `Sciclient_service`_ :

    +-----------------------+----------------------------------------------------------------+
    | opModeFlag            | Description                                                    |
    +=======================+================================================================+
    | Polling based         |  Invoked using   SCICLIENT_SERVICE_OPERATION_MODE_POLLED       |
    +-----------------------+----------------------------------------------------------------+
    | Interrupt based       | Invoked using   SCICLIENT_SERVICE_OPERATION_MODE_INTERRUPT     |
    +-----------------------+----------------------------------------------------------------+

Definition
:: 
    Defined in sciclient.h as **Sciclient_init**.


Note
::
    - This API provides a software initialization for the driver. Any application using SCIClient must call this API.
    - SCICLIENT_SERVICE_OPERATION_MODE_INTERRUPT is not supported as of now.


Typical usage
::
    Sciclient_ConfigPrms_t        config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL
    };
    Sciclient_init(&config);

.. rubric:: SYSFW communication
.. _Sciclient_service:

This API allows communicating with the System firmware which can be
called to perform various functions in the system.
Core SCIClient function for transmitting payload and recieving
the response.
The caller is expected to allocate memory for the input request
parameter **Sciclient_ReqPrm_t** . This involves setting the
message type being communicated to the firmware, the response flags,
populate the payload of the message based on the inputs in the
files located at include/tisci/ . 
Since the payload in considered a stream of bytes in this API, 
the caller should also populate the size of this stream in 
reqPayloadSize. The timeout is used to determine for what amount 
of iterations the API would wait for their operation to complete.

To make sure the response is captured correctly the caller should
also allocate the space for **Sciclient_RespPrm_t** parameters. The
caller should populate the pointer to the pRespPayload and the size
respPayloadSize. The API would populate the response flags to
indicate any firmware specific errors and also populate the memory
pointed by pRespPayload till the size given in respPayloadSize.
:: 
    Defined in sciclient.h as **Sciclient_service** .
Typical usage
::
    const Sciclient_ReqPrm_t      reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        NULL,
        0,
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t           respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };
    status = Sciclient_init(&config);
    status = Sciclient_service(&reqPrm, &respPrm);


.. rubric:: SCIClient de-initialization
.. _Sciclient_deinit:

De-initialization of sciclient. This de-initialization is specific
to the application. It only de-initializes the semaphores,
interrupts etc. which are initialized using `Sciclient_init`_ . It does
not de-initialize the system firmware.
:: 
    Defined in sciclient.h as **Sciclient_deinit** .

Typical usage
::
    Sciclient_deinit();


.. rubric:: Board configuration

One time board configuration needs to be done from R5 for rm,pm and
security to communicate with SYSFW. It is normally done by SBL, so user doesn't need to call these APIs.

User needs to define a section ".boardcfg_data" in the
linker file for the default configuration (NULL pointer to *Sciclient_BoardCfgPrms_t* is passed to the APIs), which needs to be present
in OCMRAM . If user provides custom board_cfg, then the data must
be present on OCMRAM.
:: 
    Board configuration structure is defined in include/V0/sciclient_boardcfg.h as Sciclient_BoardCfgPrms_t.

Typical usage
::
    status = Sciclient_boardCfg(NULL);
    if (CSL_PASS == status)
    {
        status = Sciclient_boardCfgPm(NULL) +
                Sciclient_boardCfgRm(NULL) +
                Sciclient_boardCfgSec(NULL) ;
    }

SCIClient Shim Layer
^^^^^^^^^^^^^^^^^^^^^^

.. rubric:: Power management
.. _Power_management:

The APIs in include/sciclient_pm.h provide declarations of SCIClient APIs for power management of the SOC .

.. rubric:: Resource management
.. _Resource_management:

The APIs in include/sciclient_rm.h provide declarations of SCIClient APIs for resource management of the SOC .


.. rubric:: Processor BOOT API Interface
.. _Processor_BOOT_API_Interface:

The APIs in include/sciclient_procboot.h provide declarations of SCIClient APIs for processor boot and control.

APIs are divided into the following two sets

- Book keeping APIs - Meant to control access to allow a reasonable usage scenario of processors.
- Processor Control APIs - Meant to be the actual Processor Corecontrols.