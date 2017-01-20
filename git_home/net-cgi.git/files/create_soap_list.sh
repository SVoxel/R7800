#!/bin/sh
. ./net-cgi.config

LIST="support_soap_list"

echo "Version: V2.00" >    $LIST
#echo "" >>    $LIST 
echo "DeviceConfig:ConfigurationStarted" >>    $LIST 
echo "DeviceConfig:ConfigurationFinished" >>    $LIST 
echo "DeviceConfig:SetEnable" >>    $LIST 
echo "DeviceConfig:Reboot" >>    $LIST 
echo "DeviceConfig:SetGUILanguage" >>    $LIST
echo "DeviceConfig:SetConfiguration" >>    $LIST 
echo "DeviceConfig:Loaddefault" >>    $LIST 
echo "DeviceConfig:SetTimeZone" >>    $LIST 
echo "DeviceConfig:SetFirmware" >>    $LIST 
echo "DeviceConfig:SetBlockSiteEnable" >>    $LIST 
echo "DeviceConfig:SetBlockSiteName" >>    $LIST 
echo "DeviceConfig:AddStaticRoute" >>    $LIST
echo "DeviceConfig:DelStaticRoute" >>    $LIST
echo "DeviceConfig:GetStaticRouteTbl" >>    $LIST
echo "DeviceConfig:EnableTrafficMeter" >>    $LIST 
echo "DeviceConfig:GetTrafficMeterEnabled" >>    $LIST 
echo "DeviceConfig:SetTrafficMeterOptions" >>    $LIST 
echo "DeviceConfig:GetTrafficMeterOptions" >>    $LIST 
echo "DeviceConfig:GetInfo" >>    $LIST 
echo "DeviceConfig:GetTimeZoneInfo" >>    $LIST 
echo "DeviceConfig:GetConfigInfo" >>    $LIST 
echo "DeviceConfig:GetBlockSiteInfo" >>    $LIST 
echo "DeviceConfig:GetTrafficMeterStatistics" >>    $LIST 
echo "DeviceConfig:CheckNewFirmware" >>    $LIST 
echo "DeviceConfig:UpdateNewFirmware" >>    $LIST 
echo "DeviceConfig:ResetToFactoryDefault" >>    $LIST 
echo "DeviceConfig:IsDLNASupported" >>    $LIST 
echo "DeviceConfig:IsDLNAEnabled" >>    $LIST 
echo "DeviceConfig:SetDLNAStatus" >>    $LIST 
echo "DeviceConfig:GetQoSEnableStatus" >>    $LIST 
echo "DeviceConfig:SetQoSEnableStatus" >>    $LIST 
echo "DeviceConfig:GetBandwidthControlEnableStatus" >>    $LIST 
echo "DeviceConfig:SetBandwidthControlEnableStatus" >>    $LIST 
echo "DeviceConfig:UpdateAdminPassword" >>    $LIST 
echo "DeviceConfig:GetQoSRules" >>    $LIST 
echo "DeviceConfig:UpdateQoSPriority" >>    $LIST 
echo "DeviceConfig:AddQoSRuleByMAC" >>    $LIST 
echo "DeviceConfig:AddQoSRuleByEthernetPort" >>    $LIST 
echo "DeviceConfig:AddQoSRule" >>    $LIST 
echo "DeviceConfig:DeleteQoSRule" >>    $LIST 
#echo "" >>    $LIST 
if [ $HAVE_ACCESS_CONTROL = 1 ]; then
echo "DeviceConfig:SetBlockDeviceEnable" >>    $LIST 
echo "DeviceConfig:GetBlockDeviceEnableStatus" >>    $LIST 
echo "DeviceConfig:EnableBlockDeviceForAll" >>    $LIST 
echo "DeviceConfig:GetBlockDeviceStateByDefault" >>    $LIST 
echo "DeviceConfig:SetBlockDeviceStateByDefault" >>    $LIST 
echo "DeviceConfig:SetBlockDeviceByMAC" >>    $LIST 
echo "DeviceConfig:GetDeviceListByMode" >>    $LIST 
echo "DeviceConfig:GetDeviceListAll" >>    $LIST 
echo "DeviceConfig:DeleteBlockDeviceByMAC" >>    $LIST 
fi

#echo "" >>    $LIST 
echo "DeviceInfo:GetInfo" >>    $LIST 
echo "DeviceInfo:GetSysUpTime" >>    $LIST 
if [ $HAVE_ACCESS_CONTROL = 1 ]; then
echo "DeviceInfo:GetAttachDevice" >>    $LIST 
fi
echo "DeviceInfo:GetSystemLogs" >>    $LIST 
echo "DeviceInfo:GetSystemInfo" >>    $LIST 
#echo "" >>    $LIST
if [ $STREAM_BOOST = 1 ]; then
echo "DeviceInfo:GetSupportFeatureList" >>    $LIST
echo "DeviceInfo:GetAttachDevice2" >>    $LIST
echo "DeviceInfo:SetDeviceNameIconByMac" >>    $LIST
fi
#echo "" >>    $LIST 
echo "WANIPConnection:SetConnectionType" >>    $LIST 
echo "WANIPConnection:SetIPInterfaceInfo" >>    $LIST 
echo "WANIPConnection:SetMACAddress" >>    $LIST 
echo "WANIPConnection:SetSmartWizardDetection" >>    $LIST 
echo "WANIPConnection:SetMaxMTUSize" >>    $LIST 
#echo "" >>    $LIST 
echo "WANIPConnection:GetConnectionTypeInfo" >>    $LIST
echo "WANIPConnection:GetInfo" >>    $LIST 
echo "WANIPConnection:GetPortMappingInfo" >>    $LIST
echo "WANIPConnection:GetInternetPortInfo" >>    $LIST
echo "WANIPConnection:AddPortMapping" >>    $LIST 
echo "WANIPConnection:DeletePortMapping" >>    $LIST 
echo "WANIPConnection:GetRemoteManagementEnableStatus" >>    $LIST 
echo "WANIPConnection:SetRemoteManagementEnable" >>    $LIST 
#echo "" >>    $LIST
if [ $HAVE_BROADBAND = 1 ]; then
echo "WAN3GInterfaceConfig:SetInterfaceOrder" >>    $LIST
echo "WAN3GInterfaceConfig:SetConnectionType" >>    $LIST
echo "WAN3GInterfaceConfig:SetWirelessBroadbandConfig" >>    $LIST
echo "WAN3GInterfaceConfig:GetInfo" >>    $LIST
echo "WAN3GInterfaceConfig:GetWirelessBroadbandInfo" >>    $LIST
echo "WAN3GInterfaceConfig:GetSIMStatus" >>    $LIST
echo "WAN3GInterfaceConfig:GetModemStatus" >>    $LIST
echo "WAN3GInterfaceConfig:GetCountryISPList" >>    $LIST
fi

#echo "" >>    $LIST 
echo "WANEthernetLinkConfig:GetEthernetLinkStatus" >>    $LIST 
echo "WANEthernetLinkConfig:SetWANRelease" >>    $LIST 
echo "WANEthernetLinkConfig:SetWANRenew" >>    $LIST 
#echo "" >>    $LIST 
echo "LANConfigSecurity:SetConfigLANSubnet" >>    $LIST 
echo "LANConfigSecurity:SetConfigLANIP" >>    $LIST
echo "LANConfigSecurity:SetConfigDHCPEnabled" >>    $LIST
echo "LANConfigSecurity:SetConfigLAN" >>    $LIST
echo "LANConfigSecurity:SetConfigPassword" >>    $LIST 
echo "LANConfigSecurity:GetInfo" >>    $LIST 
#echo "" >>    $LIST 
echo "WLANConfiguration:SetEnable" >>    $LIST 
echo "WLANConfiguration:SetConfigPassword" >>    $LIST 
echo "WLANConfiguration:SetChannel" >>    $LIST 
echo "WLANConfiguration:SetSSIDBroadcast" >>    $LIST 
echo "WLANConfiguration:SetSSID" >>    $LIST 
echo "WLANConfiguration:SetWLANNoSecurity" >>    $LIST 
echo "WLANConfiguration:SetWLANWEPByKeys" >>    $LIST 
echo "WLANConfiguration:SetWLANWEPByPassphrase" >>    $LIST 
echo "WLANConfiguration:SetWLANWPAPSKByPassphrase" >>    $LIST 
echo "WLANConfiguration:SetWPSMode" >>    $LIST 
echo "WLANConfiguration:PressWPSPBC" >>    $LIST 
echo "WLANConfiguration:SetGuestAccessNetwork" >>    $LIST 
echo "WLANConfiguration:SetGuestAccessEnabled2" >>    $LIST 
echo "WLANConfiguration:SetGuestAccessEnabled" >>    $LIST 
echo "WLANConfiguration:GetInfo" >>    $LIST 
echo "WLANConfiguration:Is5GSupported" >>    $LIST 
echo "WLANConfiguration:GetSSIDBroadcast" >>    $LIST 
echo "WLANConfiguration:GetWEPSecurityKeys" >>    $LIST 
echo "WLANConfiguration:GetWPASecurityKeys" >>    $LIST 
echo "WLANConfiguration:GetSSID" >>    $LIST 
echo "WLANConfiguration:GetChannelInfo" >>    $LIST 
echo "WLANConfiguration:GetRegion" >>    $LIST 
echo "WLANConfiguration:GetWirelessMode" >>    $LIST 
echo "WLANConfiguration:GetWPSMode" >>    $LIST 
echo "WLANConfiguration:GetWPSPINInfo" >>    $LIST 
echo "WLANConfiguration:GetGuestAccessEnabled" >>    $LIST 
echo "WLANConfiguration:GetGuestAccessNetworkInfo" >>    $LIST 
echo "WLANConfiguration:GetAvailableChannel" >>    $LIST 
echo "WLANConfiguration:GetSupportMode" >>    $LIST 
#echo "" >>    $LIST 
if [ $HAVE_WIRELESS_AN = 1 ]; then
echo "WLANConfiguration:Set5GEnable" >>    $LIST 
echo "WLANConfiguration:Set5GChannel" >>    $LIST 
echo "WLANConfiguration:Set5GSSID" >>    $LIST 
echo "WLANConfiguration:Set5GWLANNoSecurity" >>    $LIST 
echo "WLANConfiguration:Set5GWLANWEPByKeys" >>    $LIST 
echo "WLANConfiguration:Set5GWLANWEPByPassphrase" >>    $LIST 
echo "WLANConfiguration:Set5GWLANWPAPSKByPassphrase" >>    $LIST 
echo "WLANConfiguration:Set5GGuestAccessEnabled2" >>    $LIST 
echo "WLANConfiguration:Set5GGuestAccessEnabled" >>    $LIST 
echo "WLANConfiguration:Set5GGuestAccessNetwork" >>    $LIST 
echo "WLANConfiguration:Get5GInfo" >>    $LIST 
echo "WLANConfiguration:Get5GSSID" >>    $LIST 
echo "WLANConfiguration:Get5GChannelInfo" >>    $LIST 
echo "WLANConfiguration:Get5GWirelessMode" >>    $LIST 
echo "WLANConfiguration:Get5GWPASecurityKeys" >>    $LIST 
echo "WLANConfiguration:Get5GWEPSecurityKeys" >>    $LIST 
echo "WLANConfiguration:Get5GGuestAccessEnabled" >>    $LIST 
echo "WLANConfiguration:Get5GGuestAccessNetworkInfo" >>    $LIST 
fi
#echo "" >>    $LIST 
echo "Time:GetInfo" >>    $LIST 
#echo "" >>    $LIST 
if [ $DNI_PARENTAL_CTL = 1 ]; then
echo "ParentalControl:Authenticate" >> $LIST
echo "ParentalControl:GetDNSMasqDeviceID" >>    $LIST 
echo "ParentalControl:SetDNSMasqDeviceID" >>    $LIST 
echo "ParentalControl:EnableParentalControl" >>    $LIST 
echo "ParentalControl:GetAllMACAddresses" >>    $LIST 
echo "ParentalControl:DeleteMACAddress" >>    $LIST 
echo "ParentalControl:GetEnableStatus" >>    $LIST 
fi
#echo "" >>    $LIST
if [ $STREAM_BOOST = 1 ]; then
echo "AdvancedQoS:SetQoSEnableStatus" >>    $LIST
echo "AdvancedQoS:SetBandwidthControlOptions" >>    $LIST
echo "AdvancedQoS:SetDevicePriorityByMAC" >>    $LIST
echo "AdvancedQoS:GetQoSEnableStatus" >>    $LIST
echo "AdvancedQoS:GetBandwidthControlOptions" >>    $LIST
echo "AdvancedQoS:SetOOKLASpeedTestStart" >>    $LIST
echo "AdvancedQoS:GetOOKLASpeedTestResult" >>    $LIST
echo "AdvancedQoS:GetDeviceProirityByMAC" >>    $LIST
echo "AdvancedQoS:GetCurrentBandwidthByMAC" >>    $LIST
echo "AdvancedQoS:GetHistoricalTrafficByMAC" >>    $LIST
echo "AdvancedQoS:GetCurrentAppBandwidth" >>    $LIST
echo "AdvancedQoS:GetHistoricalAppTraffic" >>    $LIST
echo "AdvancedQoS:GetCurrentAppBandwidthByMAC" >>    $LIST
echo "AdvancedQoS:GetHistoricalAppTrafficByMAC" >>    $LIST
echo "AdvancedQoS:GetCurrentDeviceBandwidth" >>    $LIST
fi
