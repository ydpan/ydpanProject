#ifndef _LP_GIGEVISIONREGISTER_H_
#define _LP_GIGEVISIONREGISTER_H_

#define LP_REG_Version                                          0x0000

#define LP_REG_DeviceMode                                       0x0004

#define LP_REG_DeviceMACAddressHigh0                            0x0008

#define LP_REG_DeviceMACAddressLow0                             0x000C

#define LP_REG_NetworkInterfaceCapability0                      0x0010

#define LP_REG_NetworkInterfaceConfiguration0                   0x0014

//0x0018 Reserved - - -

#define LP_REG_CurrentIPAddress0                                0x0024

//0x0028 Reserved - - -

#define LP_REG_CurrentSubnetMask0                               0x0034

//0x0038 Reserved - - -

#define LP_REG_CurrentDefaultGateway0                           0x0044

#define LP_REG_ManufacturerName                                 0x0048

#define LP_REG_ModelName                                        0x0068

#define LP_REG_DeviceVersion                                    0x0088

#define LP_REG_ManufacturerInfo                                 0x00A8

#define LP_REG_SerialNumber                                     0x00D8

#define LP_REG_UserdefinedName                                  0x00E8

//0x00F8 Reserved - - -

#define LP_REG_FirstURL                                         0x0200

#define LP_REG_SecondURL                                        0x0400

#define LP_REG_NumberofNetworkInterfaces                        0x0600

//0x0604 Reserved - - -

#define LP_REG_PersistentIPAddress                              0x064C

//0x0650 Reserved - - -

#define LP_REG_PersistentSubnetMask0                            0x065C

//0x0660 Reserved - - -
#define LP_REG_PersistentDefaultGateway0                        0x066C

#define LP_REG_LinkSpeed0                                       0x0670

//0x0674 Reserved - - -

#define LP_REG_DeviceMACAddressHigh1                            0x0680

#define LP_REG_DeviceMACAddressLow1                             0x0684

#define LP_REG_NetworkInterfaceCapability1                      0x0688

#define LP_REG_NetworkInterfaceConfiguration1                   0x068C

//0x0690 Reserved - - -

#define LP_REG_CurrentIPAddress1                                0x069C

//0x06A0 Reserved - - -

#define LP_REG_CurrentSubnetMask1                               0x06AC

//0x06B0 Reserved - - -

#define LP_REG_CurrentDefaultGateway1                           0x06BC

//0x06C0 Reserved - - -

#define LP_REG_PersistentIPAddress1                             0x06CC

//0x06D0 Reserved - - -

#define LP_REG_PersistentSubnetMask1                            0x06DC

//0x06E0 Reserved - - -

#define LP_REG_PersistentDefaultGateway1                        0x06EC

#define LP_REG_LinkSpeed1                                       0x06F0

//0x06F4 Reserved - - -

#define LP_REG_DeviceMACAddressHigh2                            0x0700

#define LP_REG_DeviceMACAddressLow2                             0x0704

#define LP_REG_NetworkInterfaceCapability2                      0x0708

#define LP_REG_NetworkInterfaceConfiguration2                   0x070C

//0x0710 Reserved - - -

#define LP_REG_CurrentIPAddress2                                0x071C

//0x0720 Reserved - - -

#define LP_REG_CurrentSubnetMask2                               0x072C

//0x0730 Reserved - - -

#define LP_REG_CurrentDefaultGateway2                           0x073C

//0x0740 Reserved - - -

#define LP_REG_PersistentIPAddress2                             0x074C

//0x0750 Reserved - - -

#define LP_REG_PersistentSubnetMask2                            0x075C

#define LP_REG_PersistentDefaultGateway2                        0x076C

#define LP_REG_LinkSpeed2                                       0x0770

#define LP_REG_DeviceMACAddressHigh3                            0x0780

#define LP_REG_DeviceMACAddressLow3                             0x0784

#define LP_REG_NetworkInterfaceCapabilityLow3                   0x0788

#define LP_REG_NetworkInterfaceConfiguration3                   0x078C

#define LP_REG_CurrentIPAddress3                                0x079C

#define LP_REG_CurrentSubnetMask3                               0x07AC

#define LP_REG_CurrentDefaultGateway3                           0x07BC

#define LP_REG_PersistentIPAddress3                             0x07CC

#define LP_REG_PersistentSubnetMask3                            0x07DC

#define LP_REG_PersistentDefaultGateway3                        0x07EC

#define LP_REG_LinkSpeed3                                       0x07F0

#define LP_REG_NumberofMessageChannels                          0x0900

#define LP_REG_NumberofStreamChannels                           0x0904

#define LP_REG_NumberofActionSignals                            0x0908

#define LP_REG_ActionDeviceKey                                  0x090C

#define LP_REG_NumberofActiveLinks                              0x0910

#define LP_REG_GVSPCapability                                   0x092C

#define LP_REG_MessageChannelCapability                         0x0930

#define LP_REG_GVCPCapability                                   0x0934

#define LP_REG_HeartbeatTimeout                                 0x0938

#define LP_REG_TimestampTickFrequencyHigh                       0x093C

#define LP_REG_TimestampTickFrequencyLow                        0x0940

#define LP_REG_TimestampControl                                 0x0944

#define LP_REG_TimestampValueHigh                               0x0948

#define LP_REG_TimestampValueLow                                0x094C

#define LP_REG_DiscoveryACKDelay                                0x0950

#define LP_REG_GVCPConfiguration                                0x0954

#define LP_REG_PendingTimeout                                   0x0958

#define LP_REG_ControlSwitchoverKey                             0x095C

#define LP_REG_GVSPConfiguration                                0x0960

#define LP_REG_PhysicalLinkConfigurationCapability              0x0964

#define LP_REG_PhysicalLinkConfiguration                        0x0968

#define LP_REG_IEEE1588Status                                   0x096C

#define LP_REG_ScheduledActionCommandQueueSize                  0x0970

#define LP_REG_ControlChanneLP_rivilege                          0x0A00

#define LP_REG_PrimaryApplicationPort                           0x0A04

#define LP_REG_PrimaryApplicationIPAddress                      0x0A14

#define LP_REG_MessageChanneLP_ort                               0x0B00

#define LP_REG_MessageChannelDestination                        0x0B10

#define LP_REG_MessageChannelTransmissionTimeout                0x0B14

#define LP_REG_MessageChannelRetryCount                         0x0B18

#define LP_REG_MessageChannelSourcePort                         0x0B1C

#define LP_REG_StreamChanneLP_ort0                               0x0D00

#define LP_REG_StreamChanneLP_acketSize0                         0x0D04

#define LP_REG_StreamChanneLP_acketDelay0                        0x0D08

#define LP_REG_StreamChannelDestinationAddress0                 0x0D18

#define LP_REG_StreamChannelSourcePort0                         0x0D1C

#define LP_REG_StreamChannelCapability0                         0x0D20

#define LP_REG_StreamChannelConfiguration0                      0x0D24

#define LP_REG_StreamChannelZone0                               0x0D28

#define LP_REG_StreamChannelZoneDirection0                      0x0D2C

#define LP_REG_StreamChanneLP_ort1                               0x0D40

#define LP_REG_StreamChanneLP_acketSize1                         0x0D44

#define LP_REG_StreamChanneLP_acketDelay1                        0x0D48

#define LP_REG_StreamChannelDestinationAddress1                 0x0D58

#define LP_REG_StreamChannelSourcePort1                         0x0D5C

#define LP_REG_StreamChannelCapability1                         0x0D60

#define LP_REG_StreamChannelConfiguration1                      0x0D64

#define LP_REG_StreamChannelZone1                               0x0D68

#define LP_REG_StreamChannelZoneDirection1                      0x0D6C

#define LP_REG_OtherStreamChannelsRegisters                     0x0D80

#define LP_REG_StreamChanneLP_ort511                             0x8CC0

#define LP_REG_StreamChanneLP_acketSize511                       0x8CC4

#define LP_REG_StreamChanneLP_acketDelay511                      0x8CC8

#define LP_REG_StreamChannelDestinationAddress511               0x8CD8

#define LP_REG_StreamChannelSourcePort511                       0x8CDC

#define LP_REG_StreamChannelCapability511                       0x8CE0
#define LP_REG_StreamChannelConfiguration511                    0x8CE4

#define LP_REG_StreamChannelZone511                             0x8CE8

#define LP_REG_StreamChannelZoneDirection511                    0x8CEC

#define LP_REG_ManifestTable                                    0x9000

#define LP_REG_ActionGroupKey0                                  0x9800

#define LP_REG_ActionGroupMask0                                 0x9804

#define LP_REG_ActionGroupKey1                                  0x9810

#define LP_REG_ActionGroupMask1                                 0x9814

#define LP_REG_OtherActionSignalsRegisters                      0x9820

#define LP_REG_ActionGroupKey127                                0x9FF0

#define LP_REG_ActionGroupMask127                               0x9FF4

#define LP_REG_StartofNanufacturerSpecificRegisterSpace         0xA000

#endif 
