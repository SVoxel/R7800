; Copyright (c) 2013 Qualcomm Atheros, Inc..
;
; All Rights Reserved.
;
; Qualcomm Atheros Confidential and Proprietary.

classes = win,unix,other

; ==============
; MTU signatures
; ==============

[mtu]

; ==================
; TCP SYN signatures
; ==================

[tcp:request]

; -----
; Linux
; -----

label = s:unix:Linux:
sig   = *:64:0:*:mss*20,7:mss,sok,ts,nop,ws:df,id+:0
; First saw on DSM-312, but not exclusive to it
sig   = *:64:0:*:mss*4,3:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,0:mss,sok,ts:df,id+:0
sig   = *:64:0:*:mss*10,2:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,3:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,5:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,9:mss,nop,nop,sok,nop,ws:df,id+:0
sig   = *:64:0:*:mss*16,1:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*16,3:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*20,1:mss,sok,ts,nop,ws:df,id+:0
; Sonos, TiVo
sig   = *:64:0:*:mss*4,0:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,4:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,4:sok,ts,mss,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,5:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,5:sok,ts,mss,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,6:mss,nop,nop,sok,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,6:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*44,1:mss,sok,ts,nop,ws:df,id+:0
; Sling
sig   = *:64:0:*:mss*4,7:mss,sok,ts,nop,ws:df,id+:0

; Mac OUI = "Segate"
label = s:unix:Linux:Segate
sig   = *:64:0:*:mss*4,4:mss,nop,nop,sok,nop,ws:df,id+:0

; Seen on a Netgear OUI and Synology OUI
label = s:unix:Linux:PH_NetSyn
sig   = 4:64:0:*:mss*2,6:mss,sok,ts,nop,ws:df,id+:0

label = s:unix:Linux:2.x
sig   = *:64:0:*:65535,2:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:65535,3:mss,nop,nop,sok,nop,ws:df,id+:0
sig   = *:64:0:*:65535,3:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:65535,5:mss,nop,nop,sok,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,1:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,2:mss,sok,ts,nop,ws:df,id+:0

label = s:unix:Linux:3.x
sig   = *:255:0:*:mss*10,6:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:65535,6:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,1:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,4:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,6:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,7:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,8:mss,sok,ts,nop,ws:df,id+:0

; Saw this first on a Boxee device
label = s:unix:Linux:Boxee
sig   = *:64:0:*:mss*3,6:mss,sok,ts,nop,ws:df,id+:0

; Saw on a Vizio device
label = s:unix:Linux:Vizio
sig   = *:64:0:*:mss*4,4:sok,ts,mss,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,5:sok,ts,mss,nop,ws:df,id+:0

; Saw on a TiVo device
label = s:unix:Linux:TiVo
sig   = *:64:0:*:mss*3,5:mss,sok,ts,nop,ws:df,id+:0

label = s:unix:Linux:Android
sig   = *:64:0:*:mss*4,5:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*44,1:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*44,3:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:65535,7:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:65535,6:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:65535,6:mss,nop,nop,sok,nop,ws:df,id+:0
sig   = *:64:0:*:65535,3:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:65535,2:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,9:mss,nop,nop,sok,nop,ws:df,id+:0
sig   = *:64:0:*:mss*16,3:mss,sok,ts,nop,ws:df,id+:0
; Samsung
sig   = 4:64:0:*:mss*10,6:mss,nop,nop,sok,nop,ws:df,id+:0
; LG, Android 2.3.4
sig   = 4:64:0:*:49152,3:mss,sok,ts,nop,ws:df,id+:0
sig   = 4:64:0:*:32768,3:mss,sok,ts,nop,ws:df,id+:0
; Android 4
sig   = *:64:0:*:mss*20,6:mss,sok,ts,nop,ws:df,id+:0
; Android 2
sig   = *:64:0:*:mss*4,3:mss,sok,ts,nop,ws:df,id+:0

; Nest Thermostat, but also
; HTC, Android 2.3.7
label = s:unix:Linux:Nest
sig   = 4:64:0:*:mss*3,1:mss,sok,ts,nop,ws:df,id+:0

label = s:unix:Linux:Sony
sig   = 4:64:0:*:mss*4,0:mss,sok,ts:df,id+:0
sig   = 4:64:0:*:mss*44,0:mss,nop,ws,nop,nop,sok::0

label = s:unix:Linux:Roku
sig   = 4:64:0:*:mss*3,0:mss,sok,ts,nop,ws:df,id+:0
sig   = 4:64:0:*:mss*3,4:mss,sok,ts,nop,ws:df,id+:0

; -------
; Windows
; -------

label = s:win:Xbox:360
sig   = 4:64:0:*:mss*25,1:mss,nop,ws,nop,nop,sok::0
sig   = 4:64:0:*:mss*13,0:mss,nop,ws,nop,nop,sok::0

label = s:win:Windows:
sig   = *:128:0:*:32767,0:mss,nop,nop,sok:df,id+:0
sig   = *:128:0:*:mss*33,0:mss,nop,nop,sok:df,id+:0
sig   = *:64:0:*:8192,8:mss,nop,ws,nop,nop,sok:df,id+,ecn:0
sig   = *:64:0:*:65535,1:mss,nop,ws,nop,nop,ts,nop,nop,sok:df,id+,ts1-:0

label = s:win:Windows:7
sig   = *:128:0:*:8192,2:mss,nop,ws,nop,nop,sok:df,id+:0

label = s:win:Windows:8
sig   = *:128:0:*:63443,6:mss,nop,ws,nop,nop,sok:df,id+:0

label = s:win:Windows:XP
sig   = *:128:0:*:16384,0:mss,nop,nop,sok:df,id+:0
sig   = *:128:0:*:64512,0:mss,nop,nop,sok:df,id+:0
sig   = *:128:0:*:65535,0:mss,nop,nop,sok:df,id+:0
sig   = *:128:0:*:65535,0:mss,nop,ws,nop,nop,ts,nop,nop,sok:df,id+,ts1-:0
sig   = *:128:0:*:65535,1:mss,nop,ws,nop,nop,ts,nop,nop,sok:df,id+,ts1-:0
sig   = *:128:0:*:65535,1:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:128:0:*:65535,3:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:128:0:*:65535,3:mss,nop,ws,nop,nop,ts,nop,nop,sok:df,id+,ts1-:0
sig   = *:128:0:*:65535,3:mss,nop,ws,nop,nop,ts:df,id+,ts1-:0
sig   = *:128:0:*:65535,4:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:128:0:*:65535,5:mss,nop,ws,nop,nop,ts,nop,nop,sok:df,id+,ts1-:0
sig   = *:64:0:*:65535,0:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:64:0:*:65535,1:mss,nop,ws,nop,nop,ts:df,id+,ts1-:0
sig   = *:64:0:*:65535,4:mss,nop,ws,nop,nop,ts,nop,nop,sok:df,id+,ts1-:0
sig   = *:64:0:*:65535,5:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:64:0:*:mss*7,4:mss,sok,ts,nop,ws:df,id+:0

label = s:win:Windows:NT_6
sig   = *:128:0:*:32768,2:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:128:0:*:63443,8:mss,nop,ws,nop,nop,sok::0
sig   = *:128:0:*:65535,8:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:128:0:*:8192,0:mss,nop,nop,sok:df,id+,ecn:0
sig   = *:128:0:*:8192,2:mss,nop,ws,sok,ts:df,id+:0
sig   = *:128:0:*:8192,8:mss,nop,ws,nop,nop,sok:df,id+,ecn:0
sig   = *:32:0:*:8192,8:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:64:0:*:8192,2:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:64:0:*:8192,2:mss,nop,ws,sok,ts:df,id+:0

label = s:win:Windows:NT_5_or_6
sig   = *:128:0:*:16384,0:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:128:0:*:63443,6:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:128:0:*:65535,2:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:128:0:*:8192,0:mss,nop,nop,sok:df,id+:0
sig   = *:128:0:*:8192,2:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:128:0:*:8192,8:mss,nop,ws,nop,nop,sok:df,id+:0
sig   = *:64:0:*:8192,8:mss,nop,ws,nop,nop,sok:df,id+:0

; -----------
; Playstation
; -----------

label = s:other:Playstation:
sig   = 4:64:0:*:65535,0:mss,nop,ws,sok,nop,nop,nop,nop,ts:df,id+,ts1-:0
sig   = 4:64:0:*:65535,1:mss,nop,ws,sok,nop,nop,nop,nop,ts:df,id+,ts1-:0
sig   = 4:64:0:*:16384,0:mss,nop,ws,sok,nop,nop,nop,nop,ts:df,id+,ts1-:0
sig   = 4:64:0:*:65535,0:mss,nop,ws,sok,nop,nop,nop,nop,ts:df,id+:0

; ------
; Mac OS
; ------

label = s:unix:Mac_OS_X:10.x
sig   = *:64:0:*:3072,0:mss,nop,ws,nop,nop,ts,sok,eol+1:df,id+:0
sig   = *:64:0:*:4096,0:mss,nop,ws,nop,nop,ts,sok,eol+1:df,id+:0
sig   = *:64:0:*:8192,0:mss,nop,ws,nop,nop,ts,sok,eol+1:df,id+:0
sig   = *:64:0:*:65535,1:mss,nop,ws,nop,nop,ts,sok,eol+1:df,id+:0
sig   = *:64:0:*:65535,3:mss,nop,ws,nop,nop,ts,sok,eol+1:df,id+:0

label = s:unix:iOS:
sig   = *:64:0:*:65535,4:mss,nop,ws,nop,nop,ts,sok,eol+1:df,id+:0
sig   = *:64:0:*:65535,3:mss,nop,ws,?30,nop,nop,ts,sok,eol+1:df,id+:0
sig   = *:64:0:*:65535,2:mss,nop,ws,nop,nop,ts,sok,eol+1:df,id+:0
sig   = *:64:0:*:65535,0:mss,sok,eol+1:df,id+:0
sig   = *:64:0:*:32768,3:mss,nop,ws,nop,nop,ts,sok,eol+1:df,id+:0
sig   = *:64:0:*:65535,5:mss,nop,ws,nop,nop,ts,sok,eol+1:df,id+:0

; -------
; FreeBSD
; -------

label = s:unix:FreeBSD:9.x
sig   = *:64:0:*:65535,6:mss,nop,ws,sok,ts:df,id+:0

label = s:unix:FreeBSD:8.x
sig   = *:64:0:*:65535,3:mss,nop,ws,sok,ts:df,id+:0

; Catch-all rules:

label = g:unix:FreeBSD:
sig   = *:64:0:*:65535,*:mss,nop,ws,sok,ts:df,id+:0
sig   = *:64:0:*:32768,6:mss,nop,ws,sok,ts:df,id+:0

; ---
; Placeholders
; ---

label = s:other:BlackBerry:
sig   = *:128:0:1452:65535,0:mss,nop,nop,sok,nop,nop,ts::0
sig   = *:128:0:1452:65535,2:mss,nop,nop,sok,ws,nop,nop,nop,ts::0
sig   = 4:64:0:*:65535,4:mss,nop,ws,sok,nop,nop,nop,nop,ts:df,id+:0
sig   = 4:64:0:*:65535,6:mss,nop,ws,sok,nop,nop,nop,nop,ts:df,id+:0

label = s:other:Nintendo:3DS
sig   = *:64:0:1360:32768,0:mss,nop,nop,sok:df,id+:0
sig   = 4:64:0:1360:mss*24,0:mss,nop,nop,sok:df,id+:0

label = s:other:Nintendo:Wii
sig   = 4:64:0:1460:32768,0:mss,nop,nop,sok:df,id+:0

; We know this is Nintendo, but what?
label = s:other:Nintendo:Unknown
sig   = 4:64:0:1360:8192,0:mss,sok,eol+1::0
sig   = 4:64:0:1360:65535,0:mss,sok,eol+1::0
sig   = 4:64:0:1360:65535,2:mss,sok,ws,nop,eol+1::0
sig   = 4:64:0:536:8192,0:mss,sok,eol+1::0
sig   = 4:64:0:536:32768,0:mss::0
sig   = 4:64:0:536:65535,2:mss,sok,ws,nop,eol+1::0
sig   = 4:64:0:*:8192,0:mss,sok,eol+1::0
sig   = 4:64:0:*:8192,0:mss:seq-:0
sig   = 4:64:0:*:32768,0:mss::0
sig   = 4:64:0:*:65535,0:mss::0

label = s:other:Sling:main
sig   = 4:64:0:*:32000,0:mss::0

label = s:other:SunPower:
sig   = 4:64:0:1466:mss*1,0:mss:ack+:0
sig   = 4:64:0:1466:mss*1,0:mss::0

label = s:other:SiliconDust:
sig   = 4:64:0:*:mss*1,0:mss,nop,ws::0
sig   = 4:64:0:*:mss*1,0:mss::0

; Philips Lighting
label = s:other:Philips:Hue
sig   = 4:255:0:*:mss*2,0:mss::0

label = s:other:Yamaha:AVR
sig   = 4:255:0:*:32767,0:mss::0

; Don't solely use this one
label = s:other:Sony:
sig   = 4:64:0:*:mss*4,3:sok,ts,mss,nop,ws:df,id+:0

; Not sure on the TTL for this one
label = s:other:SymbianOS:9
sig   = 4:128:0:*:mss*44,1:mss,nop,nop,ts,nop,ws,nop,nop,sok:df,id+:0
sig   = 4:255:0:*:mss*44,1:mss,nop,nop,ts,nop,ws,nop,nop,sok:df,id+:0

label = s:other:PH_Linksys:
sig   = 4:255:0:*:16000,0:mss::0

label = s:other:PH_Obihai:
sig   = 4:80:0:*:16000,0:mss::0

; Seen on HP printers, Solarcity gateway (*nix?)
; Some HP printers only talk XMPP (xmpp.hpeprint.com)
label = s:other:PH_1:
sig   = 4:64:0:*:mss*6,0:mss,nop,ws,nop,nop,sok,nop,nop,ts:df,id+:0

; eQ-3 (Home automation), Gigaset (VoIP Handset)
label = s:other:PH_2:
sig   = *:128:0:*:mss*3,0:mss:df,id+:*

; Seen on an Apple OUI
label = s:other:PH_Apple:
sig   = 4:64:0:*:32768,0:mss,nop,ws,sok,nop,nop,nop,nop,ts:df,id+,ts1-:0

; Seen on a Kodak? device (Wanlida OUI)
label = s:other:PH_Kodak:
sig   = 4:255:0:*:12288,0:mss:df,id+:0

; Seen in a Motorola FiOS TV Set top box
label = s:other:PH_Motorola:
sig   = 4:64:0:*:mss*13,0:mss,nop,nop,sok,nop,nop,ts:df,id+:0

; Saw on a Vonage VDV22, guessing
label = s:unix:OpenBSD:Vonage
sig   = 4:64:0:*:16384,0:mss,nop,ws,nop,nop,ts:df,id+:0

; Saw on a Shenzhen Smarteye Digital
label = s:other:PH_Smarteye:
sig   = 4:64:0:*:mss*1,0:mss,sok,ts,nop,ws:df,id+:0

; La Crosse Technology
label = s:other:LaCrosse:
sig   = 4:255:0:*:mss*4,0:mss::0

; Arcadyan Technology Corporation
label = s:other:PH_Arcadyan:
sig   = 4:64:0:*:mss*4,2:sok,ts,mss,nop,ws:df,id+:0

; Saw on a Wii U and a Slingbox
; Also seen on a Sony STR-DN1030 Wi-Fi Network A/V Receiver
label = s:other:PH_NinSling:
sig   = 4:64:0:*:8192,0:mss::0

; Frontier Silicon Radio (Noxon?)
label = s:other:PH_Frontier:
sig   = 4:60:0:1460:0,0:mss,ws,eol+0::0
sig   = 4:255:0:1460:16384,0:mss::0

; Saw on ADEMCO (only)
label = s:other:PH_ADEMCO:
sig   = 4:128:0:1460:1536,0:mss,eol+3:df,id+:0
sig   = 4:128:0:1460:1540,0:mss,eol+3:df,id+:0

; Slim Devices
label = s:other:PH_Slim:
sig   = *:64:0:*:4096,0:mss::0

; Oregon Scientific (weird!)
label = s:other:PH_Oregon:
sig   = 4:128:0:536:100,0:mss::0
sig   = 4:128:0:536:256,0:mss::0

; HomeWizard
label = s:other:PH_HomeWizard:
sig   = 4:255:0:1500:2048,0:mss::0

; ======================
; TCP SYN+ACK signatures
; ======================

[tcp:response]

; We don't use these since we don't need to fingerprint remote hosts

; ======================
; HTTP client signatures
; ======================

[http:request]

ua_os = Linux,Windows,iOS=[iPad],iOS=[iPhone],iOS=[iPod],Mac_OS_X,FreeBSD,OpenBSD,NetBSD,Solaris=[SunOS],Nintendo=[Nintendo 3DS],Playstation=[PS Vita],Playstation=[PLAYSTATION],Playstation=[PS3Update],Playstation=[PS3Application],Xbox=[XBox],Xbox=[Xbox],Xbox=[Xenon]

; -------
; Firefox
; -------

label = s:!:Firefox:10.x_or_newer
sys   = Windows,@unix
sig   = *:Host,User-Agent,Accept=[,*/*;q=],?Accept-Language=[;q=],Accept-Encoding=[gzip, deflate],?DNT=[1],Connection=[keep-alive],?Referer:Accept-Charset,Keep-Alive:Firefox/
sig   = *:Host,User-Agent,Accept=[,*/*;q=],?Accept-Language=[;q=],Accept-Encoding=[gzip, deflate],?DNT=[1],?Referer,Connection=[keep-alive]:Accept-Charset,Keep-Alive:Firefox/

label = s:!:Firefox:2x.x_or_newer
sys   = Windows,@unix
sig   = 1:Host,User-Agent=[Firefox/],Accept=[*/*],Accept-Language,Accept-Encoding=[gzip, deflate],?Referer,Connection=[keep-alive],?If-Modified-Since:Accept-Charset,Keep-Alive:Firefox/

; ---
; iOS
; ---

label = s:!:Apple_TV:
sys   = iOS
sig   = 1:User-Agent=[(Apple TV; U; CPU OS]::(Apple TV; U; CPU OS
sig   = 1:User-Agent=[iTunes-AppleTV/]::iTunes-AppleTV/
sig   = 1:Host,Connection=[keep-alive],Accept-Encoding=[gzip, deflate],User-Agent=[AppleTV3,1/],Accept-Language=[en-us],Accept=[*/*]:Accept-Charset,Keep-Alive:AppleTV3,1/
sig   = 1:Host,Connection=[keep-alive],Accept-Encoding=[gzip, deflate],User-Agent=[AppleTV3,2/],Accept-Language=[en-us],Accept=[*/*]:Accept-Charset,Keep-Alive:AppleTV3,2/
sig   = 1:Host,Connection=[keep-alive],Accept-Encoding=[gzip, deflate],User-Agent=[AppleTV2,1/],Accept-Language=[it-it],Accept=[*/*]:Accept-Charset,Keep-Alive:AppleTV2,1/


; Note that the semicolons at the end of the sigs are part of the user agent
label = s:!:iPhone:4
sys   = iOS
sig   = 1:User-Agent=[iPhone3,1]::iPhone3,1
sig   = 1:User-Agent=[iPhone3,2]::iPhone3,2
sig   = 1:User-Agent=[iPhone3,3]::iPhone3,3

label = s:!:iPhone:4s
sys   = iOS
sig   = 1:User-Agent=[iPhone4,1]::iPhone4,1

label = s:!:iPhone:5
sys   = iOS
sig   = 1:User-Agent=[iPhone5,1]::iPhone5,1
sig   = 1:User-Agent=[iPhone5,2]::iPhone5,2

label = s:!:iPhone:5c
sys   = iOS
sig   = 1:User-Agent=[iPhone5,3]::iPhone5,3
sig   = 1:User-Agent=[iPhone5,4]::iPhone5,4

label = s:!:iPhone:5s
sys   = iOS
sig   = 1:User-Agent=[iPhone6,1]::iPhone6,1
sig   = 1:User-Agent=[iPhone6,2]::iPhone6,2

label = s:!:iPhone:6plus
sys   = iOS
sig   = 1:User-Agent=[iPhone7,1]::iPhone7,1

label = s:!:iPhone:6
sys   = iOS
sig   = 1:User-Agent=[iPhone7,2]::iPhone7,2

label = s:!:iPhone:6s
sys   = iOS
sig   = 1:User-Agent=[iPhone8,1]::iPhone8,1

label = s:!:iPhone:6splus
sys   = iOS
sig   = 1:User-Agent=[iPhone8,2]::iPhone8,2

label = s:!:iPad:2
sys   = iOS
sig   = 1:User-Agent=[iPad2,1]::iPad2,1
sig   = 1:User-Agent=[iPad2,2]::iPad2,2
sig   = 1:User-Agent=[iPad2,3]::iPad2,3
sig   = 1:User-Agent=[iPad2,4]::iPad2,4

label = s:!:iPad:Retina
sys   = iOS
sig   = 1:User-Agent=[iPad3,1]::iPad3,1
sig   = 1:User-Agent=[iPad3,2]::iPad3,2
sig   = 1:User-Agent=[iPad3,3]::iPad3,3
sig   = 1:User-Agent=[iPad3,4]::iPad3,4
sig   = 1:User-Agent=[iPad3,5]::iPad3,5
sig   = 1:User-Agent=[iPad3,6]::iPad3,6

label = s:!:iPad:Air
sys   = iOS
sig   = 1:User-Agent=[iPad4,1]::iPad4,1
sig   = 1:User-Agent=[iPad4,2]::iPad4,2

label = s:!:iPad:Air2
sys   = iOS
sig   = 1:User-Agent=[iPad5,3]::iPad5,3
sig   = 1:User-Agent=[iPad5,4]::iPad5,4

label = s:!:iPad:Mini
sys   = iOS
sig   = 1:User-Agent=[iPad2,5]::iPad2,5
sig   = 1:User-Agent=[iPad2,6]::iPad2,6
sig   = 1:User-Agent=[iPad2,7]::iPad2,7

label = s:!:iPad:Mini2
sys   = iOS
sig   = 1:User-Agent=[iPad4,4]::iPad4,4
sig   = 1:User-Agent=[iPad4,5]::iPad4,5

label = s:!:iPad:Mini3
sys   = iOS
sig   = 1:User-Agent=[iPad4,7]::iPad4,7
sig   = 1:User-Agent=[iPad4,8]::iPad4,8
sig   = 1:User-Agent=[iPad4,9]::iPad4,9

label = s:!:iPad:Mini4
sys   = iOS
sig   = 1:User-Agent=[iPad5,1]::iPad5,1
sig   = 1:User-Agent=[iPad5,2]::iPad5,2

label = s:!:iPad:Pro
sys   = iOS
sig   = 1:User-Agent=[iPad6,8]::iPad6,8

label = s:!:iPad:
sys   = iOS
sig   = 1:User-Agent=[iPad;]::iPad;
sig   = 1:User-Agent=[model/iPad]::model/iPad
sig   = 1:User-Agent=[iTunes-iPad]::iTunes-iPad/

label = s:!:iPhone:
sys   = iOS
sig   = 1:User-Agent=[iPhone;]::iPhone;
sig   = 1:User-Agent=[model/iPhone]::model/iPhone
sig   = 1:User-Agent=[iTunes-iPhone]::iTunes-iPhone/
sig   = 1:User-Agent=[iPhone OS 11]::iPhone
sig   = 1:User-Agent=[Version/11.0 Mobile]::iPhone

label = s:!:iPod:Touch
sys   = iOS
sig   = 1:User-Agent=[iPod;]::iPod;
sig   = 1:User-Agent=[iPod4,1]::iPod4,1
sig   = 1:User-Agent=[iPod5,1]::iPod5,1
sig   = 1:User-Agent=[model/iPod]::model/iPod
sig   = 1:User-Agent=[iTunes-iPod]::iTunes-iPod/
sig   = 1:Host,?Range,X-Playback-Session-Id,Accept-Encoding,Accept,Accept-Language,Connection=[keep-alive],User-Agent=[iPod touch;]:Accept-Charset,Keep-Alive:iPod touch

; ----
; MSIE
; ----

label = s:!:MSIE:XboxOne
sys   = Windows
sig   = 1:Accept=[*/*],Accept-Language,UA-CPU=[AMD64],Accept-Encoding=[gzip, deflate],User-Agent=[Xbox One],Host,?DNT,Connection=[Keep-Alive]:Accept-Charset,Keep-Alive:Xbox One
sig   = 1:User-Agent=[Mozilla/5.0 (Windows Phone 10.0; Android 6.0.1)]::Windows Phone
sig   = 1:User-Agent=[MSIE 10.0; Windows NT 6.2; Trident/6.0; MSAppHost/1.0)]::Trident
sig   = 1:User-Agent=[XBLWIN10]::XBLWIN10


label = s:!:MSIE:WinRT
sys   = Windows
sig   = 1:Accept,Accept-Language,?Referer,x-flash-version,UA-CPU=[ARM],Accept-Encoding=[gzip, deflate],User-Agent,Host,?DNT,Connection=[Keep-Alive]:Accept-Charset,Keep-Alive:Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; ARM; Trident/6.0; Touch)
sig   = 1:Accept,?Referer,Accept-Language,User-Agent,UA-CPU=[ARM],Accept-Encoding=[gzip, deflate],Host,?DNT,Connection=[Keep-Alive],?Cookie:Accept-Charset,Keep-Alive:Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; ARM; Trident/6.0; Touch)

label = s:!:MSIE:11
sys   = Windows
sig   = 0:User-Agent=[Mozilla/4.0 (compatible; MSIE 11.],Host,Connection=[Keep-Alive],?Cookie:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Mozilla/4.0 (compatible; MSIE 11.
sig   = 1:User-Agent=[Mozilla/4.0 (compatible; MSIE 11.],Host,Connection=[Keep-Alive],?Cookie:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Mozilla/4.0 (compatible; MSIE 11.

label = s:!:MSIE:10
sys   = Windows
sig   = 1:Accept,?Referer,Accept-Language,User-Agent=[(compatible; MSIE 10],Accept-Encoding=[gzip, deflate],Host,?DNT,Connection=[Keep-Alive]:Accept-Charset,Keep-Alive:(compatible; MSIE 10
sig   = 1:Accept=[*/*],Accept-Language,?Referer,?x-flash-version,Accept-Encoding=[gzip, deflate],User-Agent=[(compatible; MSIE 10],Host,?If-Modified-Since,?DNT,Connection=[Keep-Alive]:Accept-Charset,Keep-Alive:(compatible; MSIE 10

label = s:!:MSIE:9
sys   = Windows
sig   = 1:Accept-Language,User-Agent=[Mozilla/4.0 (compatible; MSIE 9],Host,Connection=[Keep-Alive]:Accept,Accept-Encoding,Accept-Charset,Keep-Alive:Mozilla/4.0 (compatible; MSIE 9
sig   = 1:Host,User-Agent=[Mozilla/5.0 (compatible; MSIE 9],Accept-Encoding=[gzip],Accept-Language,?Cookie,Connection=[Close]:Accept,Accept-Charset,Keep-Alive:Mozilla/5.0 (MSIE 9
sig   = 1:Connection=[Keep-Alive],Pragma=[no-cache],Accept=[*/*],User-Agent=[Mozilla/4.0 (compatible; MSIE 9],Host:Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Mozilla/4.0 (compatible; MSIE 9

label = s:!:MSIE:8
sys   = Windows
sig   = 1:Accept=[*/*],?Referer,?Accept-Language,User-Agent=[Mozilla/4.0 (compatible; MSIE 8],Accept-Encoding=[gzip, deflate],Host,Connection=[Keep-Alive]:Keep-Alive,Accept-Charset,UA-CPU:Mozilla/4.0 (compatible; MSIE 8
sig   = 1:Accept=[*/*],?Referer,?Accept-Language,Accept-Encoding=[gzip, deflate],User-Agent=[Mozilla/4.0 (compatible; MSIE 8],Host,Connection=[Keep-Alive]:Keep-Alive,Accept-Charset:Mozilla/4.0 (compatible; MSIE 8

label = s:!:MSIE:7
sys   = Windows
sig   = 1:Accept=[*/*],?Referer,?Accept-Language,?UA-CPU,User-Agent=[(compatible; MSIE 7],Accept-Encoding=[gzip, deflate],Host,Connection=[Keep-Alive]:Keep-Alive,Accept-Charset:(compatible; MSIE 7
sig   = 1:Accept=[*/*],?Referer,Accept-Language,Accept-Encoding=[gzip, deflate],User-Agent=[Mozilla/4.0 (compatible; MSIE 7],Host,Connection=[Keep-Alive],?Cookie:Accept-Charset,Keep-Alive:Mozilla/4.0 (compatible; MSIE 7

; TODO: Check if this one ever uses Accept-Language, etc. Also try to find MSIE 5.

label = s:!:MSIE:6
sys   = Windows
sig   = 0:Accept=[*/*],?Referer,User-Agent,Host:Keep-Alive,Connection,Accept-Encoding,Accept-Language,Accept-Charset:(compatible; MSIE
sig   = 1:Accept=[*/*],Connection=[Keep-Alive],Host,?Pragma=[no-cache],?Range,?Referer,User-Agent:Keep-Alive,Accept-Encoding,Accept-Language,Accept-Charset:(compatible; MSIE

label = s:!:DynGate:
sys   = Windows
sig   = 1:User-Agent=[Mozilla/4.0 (compatible; MSIE 6.0; DynGate)],Host,Accept=[*/*]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Mozilla/4.0 (compatible; MSIE 6.0; DynGate)

; ------
; Chrome
; ------

label = s:!:Chrome:Chrome_OS
sys   = Linux
sig   = 1:User-Agent=[Mozilla/5.0 (X11; CrOS]::Mozilla/5.0 (X11; CrOS

label = s:!:Chrome:Android
sys   = @unix
sig   = 1:Host,Connection=[keep-alive],User-Agent=[Linux; Android],Accept=[text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8],?Referer,Accept-Encoding=[gzip,deflate,sdch],Accept-Language,Accept-Charset=[ISO-8859-1,utf-8;q=0.7,*;q=0.3],?Cookie:Keep-Alive:Linux; Android 4

label = s:!:Chrome:11_or_newer
sys   = Windows,@unix
sig   = 1:Host,Connection=[keep-alive],User-Agent,Accept=[*/*],?Referer,Accept-Encoding=[gzip,deflate,sdch],Accept-Language,Accept-Charset=[utf-8;q=0.7,*;q=0.3]:: Chrom
sig   = 1:Host,Connection=[keep-alive],User-Agent,Accept=[*/*],?Referer,Accept-Encoding=[gzip,deflate,sdch],Accept-Language,Accept-Charset=[UTF-8,*;q=0.5]:: Chrom
sig   = 1:Host,User-Agent,Accept=[*/*],?Referer,Accept-Encoding=[gzip,deflate,sdch],Accept-Language,Accept-Charset=[utf-8;q=0.7,*;q=0.3],Connection=[keep-alive]::Chrom
sig   = 1:Host,Connection=[keep-alive],?Accept,?X-Rlz-String,User-Agent=[Chrome],?Referer,Accept-Encoding=[gzip,deflate,sdch],Accept-Language:Accept-Charset,Keep-Alive:Chrome
sig   = 1:Host,Connection=[keep-alive],?Cache-Control,?Accept,User-Agent=[Chrome],X-Rlz-String,?X-Chrome-Variations,Accept-Encoding=[gzip,deflate,sdch],Accept-Language=[en-US,en;q=0.8],?Cookie:Accept-Charset,Keep-Alive:Chrome

; --------------
; Model-specific Android browser
; ---------------

; Nexus S
label = s:!:Nexus:S
sys   = Linux
sig   = 1:User-Agent=[; Nexus S 4G Build]::Nexus S 4G Build
sig   = 1:User-Agent=[; Nexus S Build]::Nexus S Build

; Galaxy S
label = s:!:Samsung:GalaxyS
sys   = Linux
sig   = 1:User-Agent=[SCH-R760]::SCH-R760

; Galaxy S II
label = s:!:Samsung:GalaxySII
sys   = Linux
sig   = 1:User-Agent=[GT-I9100]::GT-I9100
sig   = 1:User-Agent=[SPH-D710]::SPH-D710
sig   = 1:User-Agent=[SGH-T989]::SGH-T989

; Galaxy S III
label = s:!:Samsung:GalaxySIII
sys   = Linux
sig   = 1:User-Agent=[SCH-R530]::SCH-R530
sig   = 1:User-Agent=[GT-I9300]::GT-I9300
sig   = 1:User-Agent=[GT-I9305]::GT-I9305
sig   = 1:User-Agent=[SHV-E210]::SHV-E210
sig   = 1:User-Agent=[SGH-T999]::SGH-T999
sig   = 1:User-Agent=[SGH-I747]::SGH-I747
sig   = 1:User-Agent=[SGH-N064]::SGH-N064
sig   = 1:User-Agent=[SGN-N035]::SGN-N035
sig   = 1:User-Agent=[SCH-J021]::SCH-J021
sig   = 1:User-Agent=[SCH-I535]::SCH-I535
sig   = 1:User-Agent=[SPH-L710]::SPH-L710
sig   = 1:User-Agent=[GT-I9308]::GT-I9308
sig   = 1:User-Agent=[SCH-I939]::SCH-I939
; Galaxy S III mini
sig   = 1:User-Agent=[GT-I8190]::GT-I8190

; Galaxy S4
label = s:!:Samsung:GalaxyS4
sys   = Linux
sig   = 1:User-Agent=[SGH-M919]::SGH-M919
sig   = 1:User-Agent=[GT-I9500]::GT-I9500
sig   = 1:User-Agent=[GT-I9505]::GT-I9505
sig   = 1:User-Agent=[SHV-E300]::SHV-E300
sig   = 1:User-Agent=[GT-I9506]::GT-I9506
sig   = 1:User-Agent=[SGH-I337]::SGH-I337
sig   = 1:User-Agent=[SCH-I545]::SCH-I545
sig   = 1:User-Agent=[SPH-L720]::SPH-L720
sig   = 1:User-Agent=[SCH-R970]::SCH-R970
sig   = 1:User-Agent=[GT-I9508]::GT-I9508
sig   = 1:User-Agent=[SCH-I959]::SCH-I959
sig   = 1:User-Agent=[GT-I9502]::GT-I9502
sig   = 1:User-Agent=[SGH-N045]::SGH-N045
; S4 Active
sig   = 1:User-Agent=[SGH-I537]::SGH-I537
sig   = 1:User-Agent=[SHV-E470S]::SHV-E470S
; S4 Zoom
sig   = 1:User-Agent=[SM-C105A]::SM-C105A

; Galaxy S4 Mini
label = s:!:Samsung:GalaxyS4Mini
sys   = Linux
sig   = 1:User-Agent=[GT-I9190]::GT-I9190
sig   = 1:User-Agent=[GT-I9195]::GT-I9195
sig   = 1:User-Agent=[GT-I9192]::GT-I9192
sig   = 1:User-Agent=[GT-I9198]::GT-I9198
sig   = 1:User-Agent=[SPH-L520]::SPH-L520
sig   = 1:User-Agent=[SCH-I435]::SCH-I435

; Galaxy S5
label = s:!:Samsung:GalaxyS5
sys   = Linux
sig   = 1:User-Agent=[SM-G900]::SM-G900

; Galaxy J3
label = s:!:Samsung:GalaxyJ3
sys   = Linux
sig   = 1:User-Agent=[SM-J327T]::SM-J327T

; Galaxy Nexus
label = s:!:Samsung:GalaxyNexus
sys   = Linux
sig   = 1:User-Agent=[Galaxy Nexus]::Galaxy Nexus

label = s:!:Samsung:GalaxyNote
sys   = Linux
sig   = 1:User-Agent=[; GT-N7000 Build]::; GT-N7000 Build
sig   = 1:User-Agent=[; SAMSUNG-SGH-I717 Build]::; SAMSUNG-SGH-I717 Build
sig   = 1:User-Agent=[GT-N7000B]::GT-N7000B
sig   = 1:User-Agent=[GT-N7005]::GT-N7005
sig   = 1:User-Agent=[SHV-E160K]::SHV-E160K
sig   = 1:User-Agent=[SHV-E160L]::SHV-E160L
sig   = 1:User-Agent=[SHV-E160S]::SHV-E160S
sig   = 1:User-Agent=[SC-05D]::SC-05D
sig   = 1:User-Agent=[SGH-T879]::SGH-T879
sig   = 1:User-Agent=[GT-I9220]::GT-I9220
sig   = 1:User-Agent=[GT-I9228]::GT-I9228
sig   = 1:User-Agent=[SCH-I889]::SCH-I889


label = s:!:Samsung:GalaxyNoteII
sys   = Linux
sig   = 1:User-Agent=[; SCH-I605 Build]::; SCH-I605 Build
sig   = 1:User-Agent=[GT-N7100]::GT-N7100
sig   = 1:User-Agent=[GT-N7102]::GT-N7102
sig   = 1:User-Agent=[GT-N7105]::GT-N7105
sig   = 1:User-Agent=[GT-N7108]::GT-N7108
sig   = 1:User-Agent=[SCH-R950]::SCH-R950
sig   = 1:User-Agent=[SGH-I317]::SGH-I317
sig   = 1:User-Agent=[SGH-T889]::SGH-T889
sig   = 1:User-Agent=[SPH-L900]::SPH-L900
sig   = 1:User-Agent=[SCH-N719]::SCH-N719
sig   = 1:User-Agent=[SGH-N025]::SGH-N025
sig   = 1:User-Agent=[SC-02E]::SC-02E
sig   = 1:User-Agent=[SHV-E250K]::SHV-E250K
sig   = 1:User-Agent=[SHV-E250L]::SHV-E250L
sig   = 1:User-Agent=[SHV-E250S]::SHV-E250S

label = s:!:Samsung:GalaxyNote3
sys   = Linux
sig   = 1:User-Agent=[SM-N900]::SM-N900

label = s:!:Samsung:GalaxyTab
sys   = Linux
sig   = 1:User-Agent=[; GT-P]::; GT-P
sig   = 1:User-Agent=[; SM-T]::; SM-T
sig   = 1:User-Agent=[; SAMSUNG-SM-T]::; SAMSUNG-SM-T

label = s:!:Samsung:GalaxyNoteTablet
sys   = Linux
sig   = 1:User-Agent=[GT-N5110]::GT-N5110
sig   = 1:User-Agent=[GT-N8013]::GT-N8013

label = s:!:Samsung:SmartTV
sys   = Linux
sig   = 1:Host,User-Agent=[Linux/SmartTV;SmartTV Safari/],Accept=[*/*],Accept-Encoding=[gzip, deflate],Connection=[Keep-Alive]:Accept-Language,Accept-Charset,Keep-Alive:Linux/SmartTV;SmartTV Safari/
sig   = 1:DUID,MACAddr,ModelId,Category,Param,CountryCode,Host,Connection=[close]:User-Agent,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:
sig   = 1:Host,Connection=[close],User-Agent=[SmartHub; SMART-TV],Content-Type=[text/xml],cache-control=[no-cache],Accept=[*/*],Accept-Encoding=[gzip, deflate],Content-Length=[0]:Accept-Language,Accept-Charset,Keep-Alive:SmartHub; SMART-TV
sig   = 1:Host,User-Agent=[SmartHub; SMART-TV],?Cookie,Accept=[*/*],Accept-Encoding=[gzip, deflate],Connection=[Keep-Alive]:Accept-Language,Accept-Charset,Keep-Alive:SmartHub; SMART-TV
sig    = 1:User-Agent=[Maple 5.1.00266 Navi]::Maple 5.1.00266 Navi
sig    = 1:User-Agent=[Maple 6.0.00077 Navi]::Maple 6.0.00077 Navi

label = s:!:Motorola:DROID_4
sys   = Linux
sig   = 1:User-Agent=[; DROID4 Build]::; DROID4 Build

label = s:!:Motorola:DROID_Bionic
sys   = Linux
sig   = 1:User-Agent=[; DROID BIONIC Build]::; DROID BIONIC Build

label = s:!:Motorola:DROID_RAZR
sys   = Linux
sig   = 1:User-Agent=[DROID RAZR Build]::DROID RAZR Build

label = s:!:Motorola:DROID_RAZR_HD
sys   = Linux
sig   = 1:User-Agent=[DROID RAZR HD Build]::DROID RAZR HD Build

label = s:!:Motorola:DROID_Ultra
sys   = Linux
sig   = 1:User-Agent=[XT1080 Build]::XT1080 Build

label = s:!:Motorola:DROID_M
sys   = Linux
sig   = 1:User-Agent=[XT907 Build]::XT907 Build

label = s:!:Motorola:Moto_X
sys   = Linux
sig   = 1:User-Agent=[XT1053 Build]::XT1053 Build
sig   = 1:User-Agent=[XT1055 Build]::XT1055 Build
sig   = 1:User-Agent=[XT1056 Build]::XT1056 Build
sig   = 1:User-Agent=[XT1060 Build]::XT1060 Build

label = s:!:Motorola:DROID_MINI
sys   = Linux
sig   = 1:User-Agent=[XT1030 Build]::XT1030 Build

label = s:!:Motorola:Moto_G
sys   = Linux
sig   = 1:User-Agent=[XT1032 Build]::XT1032 Build

label = s:!:HTC:One
sys   = Linux
sig   = 1:User-Agent=[HTC One Build]::HTC One Build
sig   = 1:User-Agent=[HTC_One Build]::HTC_One Build
sig   = 1:User-Agent=[HTC One 801e Build]::HTC One 801e Build
sig   = 1:User-Agent=[; HTCONE Build]::; HTCONE Build
sig   = 1:User-Agent=[; HTC6500LVW Build]::; HTC6500LVW Build

label = s:!:HTC:One_M8
sys   = Linux
sig   = 1:User-Agent=[HTC One_M8 Build]::HTC One_M8 Build

label = s:!:HTC:One_X
sys   = Linux
sig   = 1:User-Agent=[HTC One X Build]::HTC One X Build

label = s:!:HTC:One_S
sys   = Linux
sig   = 1:User-Agent=[; HTC_One_S Build]::; HTC_One_S Build
sig   = 1:User-Agent=[; HTC One S Build]::; HTC One S Build

label = s:!:HTC:ThunderBolt
sys   = Linux
sig   = 1:User-Agent=[ADR6400L Build]::ADR6400L Build

label = s:!:HTC:Shift_4G
sys   = Linux
sig   = 1:User-Agent=[PG06100 Build]::PG06100 Build

label = s:!:HTC:J
sys   = Linux
sig   = 1:User-Agent=[HTC_J_Z321e Build]::HTC_J_Z321e Build
sig   = 1:User-Agent=[; HTC J Z321e Build]::; HTC J Z321e Build

label = s:!:HTC:Amaze_4G
sys   = Linux
sig   = 1:User-Agent=[HTC_Amaze_4G Build]::HTC_Amaze_4G Build

label = s:!:HTC:Rhyme
sys   = Linux
sig   = 1:User-Agent=[; ADR6330VW Build]::; ADR6330VW Build

label = s:!:HTC:Rezound
sys   = Linux
sig   = 1:User-Agent=[; ADR6425LVW Build]::; ADR6425LVW Build

label = s:!:HTC:EVO
sys   = Linux
sig   = 1:User-Agent=[; EVO Build]::; EVO Build

label = s:!:HTC:Droid_DNA
sys   = Linux
sig   = 1:User-Agent=[; HTC6435LVW Build]::; EVO Build

label = s:!:Sony:Xperia_go
sys   = Linux
sig   = 1:User-Agent=[ST27i Build]::ST27i Build
sig   = 1:User-Agent=[ST27a Build]::ST27a Build

label = s:!:Sony:Xperia_Z1
sys   = Linux
sig   = 1:User-Agent=[C6903 Build]::C6903 Build
; Z1s
sig   = 1:User-Agent=[C6916 Build]::C6916 Build

label = s:!:Sony:Xperia_Z2
sys   = Linux
sig   = 1:User-Agent=[D6503 Build]::D6503 Build

label = s:!:Sony:Xperia_Z
sys   = Linux
sig   = 1:User-Agent=[; C6603 Build]::; C6603 Build
sig   = 1:User-Agent=[; C6602 Build]::; C6602 Build

label = s:!:Sony:Xperia_Tablet_Z
sys   = Linux
sig   = 1:User-Agent=[; SGP311 Build]::; SGP311 Build
sig   = 1:User-Agent=[; SGP312 Build]::; SGP312 Build
sig   = 1:User-Agent=[; SGP321 Build]::; SGP321 Build

label = s:!:Nexus:10
sys   = Linux
sig   = 1:User-Agent=[; Nexus 10 Build]::; Nexus 10 Build

label = s:!:Nexus:5
sys   = Linux
sig   = 1:User-Agent=[; Nexus 5 Build]::; Nexus 5 Build

label = s:!:Nexus:7
sys   = Linux
sig   = 1:User-Agent=[; Nexus 7 Build]::; Nexus 7 Build

label = s:!:Nexus:4
sys   = Linux
sig   = 1:User-Agent=[; Nexus 4 Build]::; Nexus 4 Build

label = s:!:Kindle:Fire
sys   = Linux
sig   = 1:User-Agent=[; KFTT Build]::; KFTT Build
sig   = 1:User-Agent=[; Kindle Fire Build]::; Kindle Fire Build

label = s:!:Kindle:Fire_HD
sys   = Linux
sig   = 1:User-Agent=[; KFJWI Build]::; KFJWI Build

label = s:!:Kindle:Fire_HDX
sys   = Linux
sig   = 1:User-Agent=[; KFTHWI Build]::; KFTHWI Build
sig   = 1:User-Agent=[; KFAPWI Build]::; KFAPWI Build

label = s:!:ASUS:MeMOPad
sys   = Linux
; HD 7-Inch
sig   = 1:User-Agent=[; ME173X Build]::; ME173X Build

label = s:!:Amazon:Fire_TV
sys   = Linux
sig   = 1:User-Agent=[; AFTB Build]::; AFTB Build

label = s:!:LG:G2
sys   = Linux
sig   = 1:User-Agent=[; LG-LS980 Build]::; LG-LS980 Build
sig   = 1:User-Agent=[; LG-D800 Build]::; LG-D800 Build

label = s:!:LG:G_Pro_2
sys   = Linux
sig   = 1:User-Agent=[; LG-D838 Build]::; LG-D838 Build

; Generic Android Tablet
label = s:!:Generic:Tablet
sys   = Linux
sig   = 1:User-Agent=[; NX008HD Build]::; NX008HD Build

; ---------------
; Android browser
; ---------------

label = s:!:Android:Google_TV
sys   = Linux
sig   = 1:?Cookie,Host,Connection=[Keep-Alive],User-Agent=[GoogleTV]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:AndroidDownloadManager
sig   = 1:Host,Connection=[keep-alive],?Referer,User-Agent=[GoogleTV],Accept=[*/*],Accept-Encoding=[gzip,deflate,sdch],Accept-Language,Accept-Charset:Keep-Alive:GoogleTV
sig   = 1:?Range,User-Agent=[GTVMediaPlayer/],Host,Accept=[*/*]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:GTVMediaPlayer/


; First one is from Google Earth
label = s:!:Android:SHIELD
sys   = Linux
sig   = 1:?Cookie,?Accept,User-Agent=[(Android;Android (SHIELD-roth-user],Connection=[Keep-Alive],Host,Accept-Encoding=[gzip]:Accept-Language,Accept-Charset,Keep-Alive:(Android;Android (SHIELD-roth-user
sig   = 1:User-Agent=[; SHIELD Build/],Host,Connection=[Keep-Alive],Accept-Encoding=[gzip]:Accept,Accept-Language,Accept-Charset,Keep-Alive:; SHIELD Build/

label = s:!:Android:2.x
sys   = Linux
sig   = 1:Host,Accept-Encoding=[gzip],Accept-Language,User-Agent=[Android],Accept=[,*/*;q=0.5],Accept-Charset=[utf-16, *;q=0.7]:Connection:Android
sig   = 1:Host,Connection=[keep-alive],Accept-Encoding=[gzip],Accept-Language,User-Agent=[Android],Accept=[,*/*;q=0.5],Accept-Charset=[utf-16, *;q=0.7]::Android
sig   = 1:Host,Accept-Encoding=[gzip],Accept-Language=[en-US],Accept=[*/*;q=0.5],User-Agent=[Android],Accept-Charset=[utf-16, *;q=0.7]:Connection:Android
sig   = 1:User-Agent=[stagefright/1.0 (Linux;Android 2.],Host,?Range:Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:stagefright/1.0 (Linux;Android 2.

label = s:!:Android:4.x
sys   = Linux
sig   = 1:Host,Connection=[keep-alive],Accept=[,*/*;q=0.8],User-Agent=[Android],Accept-Encoding=[gzip,deflate],Accept-Language,Accept-Charset=[utf-16, *;q=0.7]::Android
sig   = 1:User-Agent=[Linux; U; Android 4],Host,Connection=[Keep-Alive],Accept-Encoding=[gzip]:Accept,Accept-Language,Accept-Charset,Keep-Alive:Linux; U; Android
sig   = 1:Host,Connection=[Keep-Alive],User-Agent=[Android]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Android

; -------------------
; Android Apps
; -------------------

label = s:!:Android:stagefright
sys   = Linux
sig   = 1:Host,Connection=[keep-alive],User-Agent=[Linux;Android],Accept-Encoding=[gzip,deflate]:Accept,Accept-Language,Accept-Charset,Keep-Alive:Linux;Android 4

label = s:!:reddit:Android
sys   = Linux
sig   = 1:Host,Connection=[Keep-Alive],User-Agent=[reddit is fun (Android)],Accept-Encoding=[gzip],?Cache-Control:Accept,Accept-Language,Accept-Charset,Keep-Alive:reddit is fun (Android)

label = s:!:netflix:WDTV
sys   = Linux
sig   = 1:User-Agent=[(DEVTYPE=WDBHG70000; CERTVER=0)],X-NBP-UIClient=[netflix-webkit],X-NBP-UIClient-BuildDebug=[false],X-NBP-UIClient-BuildProduction=[true],Connection=[Keep-Alive],Accept-Encoding=[gzip],Accept-Language,Host:Accept,Accept-Charset,Keep-Alive:(DEVTYPE=WDBHG70000; CERTVER=0)

label = s:!:WD:WDTV
sys   = Linux
sig   = 1:?Origin,User-Agent=[(QtEmbedded; U; Linux; C) WDTVLive/],Accept=[*/*],Connection=[Keep-Alive],Accept-Encoding=[gzip],Accept-Language=[en,*],Host:Accept-Charset,Keep-Alive:(QtEmbedded; U; Linux; C) WDTVLive/

; The Weather Channel for Android
label = s:!:Android:TWC
sys   = Linux
sig   = 1:Host,Connection=[Keep-Alive],User-Agent=[Google/TWC],Accept-Encoding=[gzip]:Accept,Accept-Language,Accept-Charset,Keep-Alive:Google/TWC

label = s:!:Android:HTCSP
sys   = Linux
sig   = 1:Host,Connection=[keep-alive],Bandwidth,User-Agent=[HTC Streaming Player],x-htc-autoclose,x-htc-buffertime,x-htc-internetwarning,x-htc-maxudpport,x-htc-minudpport,x-htc-romversion,x-htc-rtcpreportinterval,x-htc-rtptimeout,x-htc-rtspkeepaliveinterval,x-htc-rtsptimeout,x-network-type,x-wap-profile,Accept-Encoding=[gzip,deflate],?Range:Accept,Accept-Language,Accept-Charset,Keep-Alive:HTC Streaming Player

label = s:!:Android:BeyondPod
sys   = Linux
sig   = 1:Accept=[*/*],Accept-Encoding=[gzip],?Range,Host,Connection=[Keep-Alive],User-Agent=[BeyondPod]:Accept-Language,Accept-Charset,Keep-Alive:BeyondPod

label = s:!:Android:generic
sys   = Linux
sig   = 1:User-Agent=[Android]::Android

; ----------
; BlackBerry
; ----------

label = s:!:BlackBerry:9220
sys   = BlackBerry
sig   = 1:Host,Connection=[Keep-Alive],?Cache-Control,User-Agent,X-Wap-Proxy-Cookie=[none],SP-UA=[BlackBerry 9220]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:BlackBerry 9220


; -------------------
; Playstation 3 browser
; -------------------

label = s:!:Playstation:3
sys   = Playstation
sig   = 1:Host,Connection=[Keep-Alive],?Cookie,Accept-Encoding=[identity],x-ps3-browser,Accept-Language,User-Agent=[PLAYSTATION 3]:Accept,Accept-Charset,Keep-Alive:PLAYSTATION 3
sig   = 1:Host,User-Agent=[PS3Application],Connection=[Keep-Alive],Accept-Encoding=[identity]:Accept,Accept-Language,Accept-Charset,Keep-Alive:PS3Application
sig   = 1:Host,Connection=[Keep-Alive],Accept-Encoding=[identity],User-Agent=[PS3Update-agent]:Accept,Accept-Language,Accept-Charset,Keep-Alive:PS3Update-agent
sig   = 1:User-Agent=[Playstation3]::Playstation3
sig   = 1:User-Agent=[PS3ThumnailGet]::PS3ThumnailGet
sig   = 1:Accept=[*/*],Host,?Range,X-Device=[NFPS3-]:User-Agent,Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

; -------------------
; Playstation 4 browser
; -------------------
label = s:!:Playstation:4
sys   = FreeBSD
sig   = 1:User-Agent=[(PlayStation 4)]::(PlayStation 4)

; -------------------
; PS Vita browser
; -------------------
label = s:!:Playstation:Vita
sys   = Playstation
sig   = 1:?If-Modified-Since,Host,User-Agent=[PS Vita],Connection=[Keep-Alive]:Accept,Accept-Encoding,Accept-Charset,Keep-Alive:PS Vita

; --------------------
; Nintendo
; --------------------

label = s:!:Nintendo:3DS
sys   = Nintendo
sig   = 1:Host,User-Agent,HTTP_X_GAMEID,Content-Type=[application/x-www-form-urlencoded],Connection=[Close]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:CTR
sig   = 1:User-Agent=[Nintendo 3DS]::Nintendo 3DS
; Specified on Nintendo's web site

label = s:!:Nintendo:WiiU
sys   = @other
sig   = 1:Accept=[*/*],Host,?Range,X-Device=[NFWIIU-]:User-Agent,Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:
sig   = 1:User-Agent=[Nintendo WiiU/],Host,Accept=[*/*],?Cookie,?Range:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Nintendo WiiU/
sig   = 1:User-Agent=[PBOSU-3.0/],Host,Accept=[*/*]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:PBOSU-3.0/
sig   = 1:Host=[pushmore.wup.shop.nintendo.net],Accept=[*/*]:User-Agent,Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

; ---------
; WebClient
; ---------

label = s:!:WebClient:Xbox
sys   = Xbox
sig   = 1:User-Agent=[Mozilla/4.0 (compatible; NativeHost)],Connection=[Keep-alive],Host,?Referer:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Mozilla/4.0 (compatible; NativeHost)

; ---------
; Xbox 360
; ---------

label = s:!:Xbox:360
sys   = Xbox
sig   = 1:User-Agent=[Xbox/2],Connection=[Keep-alive],Host:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Xbox/2
sig   = 1:User-Agent=[Xbox 360],Connection=[Keep-alive],Host:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Xbox 360
sig   = 1:User-Agent=[Xbox Live Client/2],Connection=[Keep-alive],Host:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Xbox Live Client/2
sig   = 1:User-Agent=[Xenon],Connection=[Keep-alive],Host,RANGE=[bytes=]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Xenon
sig   = 1:User-Agent=[Zune Video (Xbox)],Connection=[Keep-alive],Host:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Zune Video (XBox)

; ---------
; Mac App Store
; ---------

label = s:!:Mac_OS_X:AppStore
sys   = @unix
sig   = 1:Host,User-Agent=[(x86_64) (Mac],Accept=[*/*],Accept-Language,Accept-Encoding=[gzip, deflate],Connection=[keep-alive]:Accept-Charset,Keep-Alive:(x86_64) (Mac

; ---------
; Mac Safari and Chrome
; ---------

label = s:!:Mac_OS_X:Safari_or_Chrome
sys   = @unix
sig   = 1:User-Agent=[(Macintosh;]::(Macintosh;

; ---------
; Killer Network Manager
; ---------

label = s:!:Killer_Network_Manager:
sys   = @win
sig   = 1:User-Agent=[Bigfoot-Killer Network Manager/],Connection=[Keep-Alive],Accept-Encoding=[gzip],Accept-Language=[en-US,*],Host:Accept,Accept-Charset,Keep-Alive:Bigfoot-Killer Network Manager/

; ------
; Linksys Phone Adapter
; ------

label = s:!:Linksys:PhoneAdapter
sys   = PH_Linksys
sig   = 0:Host,User-Agent=[Linksys/SPA],Connection=[close]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Linksys/SPA

; ------
; Obihai
; ------

label = s:!:Obihai:VoIP
sys   = PH_Obihai
sig   = 1:Host,User-Agent=[OBi],?Cache-Control,Accept-Language=[en-us, en],Accept-Charset=[ISO-8859-1, utf-8],Connection=[Close]:Accept,Accept-Encoding,Keep-Alive:OBi

; --
; HP
; --

label = s:!:HP:InkjetUpdate
sys   = PH_1
sig   = 1:Host,Accept-Encoding=[identity;q=1.0, *;q=0],User-Agent=[HP-Inkjet-WebUpdate]:Connection,Accept,Accept-Language,Accept-Charset,Keep-Alive:HP-Inkjet-WebUpdate

; ----
; TiVo
; ----

label = s:!:TiVo:
sys   = Linux
sig   = 0:User-Agent,Host,TiVo_TCD_ID,TiVo_SW_VER:Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:TmkHttpRequest/1.0
sig   = 0:User-Agent,TiVo_TCD_ID,TiVo_SW_VER:Host,Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:TmkHttpRequest/1.0

; ------
; Vonage
; ------

label = s:!:Vonage:VDV2
sys   = OpenBSD
sig   = 1:User-Agent=[VDV2],Host,Connection=[Keep-Alive]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:VDV2

; -----
; Sling
; -----
; These may not be specific enough, but we require OS match too

label = s:!:Sling:
sys   = Sling
sig   = 1:User-Agent,Host,Accept=[application/json]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

; -----
; Kodak
; -----

label = s:!:AVOS:
sys   = Kodak
sig   = 1:User-Agent=[AVOS],Host,DeviceToken:Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:AVOS/1.1 libhttp/1.1

; --------
; Motorola
; --------

label = s:!:Motorola:DCT
sys   = PH_Motorola
sig   = 1:Host,User-Agent=[Motorola-DCT],Connection=[close]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Motorola-DCT

; --------
; Magicjack
; --------
label = s:!:Magicjack:
sys   = OpenBSD
sig   = 0:Connection=[Keep-Alive],Host,User-Agent,Content-Length=[0]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:mJ

; -------
; DirecTV
; -------
label = s:!:DirecTV:
sys   = Linux
sig   = 0:Accept=[text/*, image/*, video/*],Host=[dtvbb.tv]:User-Agent,Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

; -----
; Frontier Silicon
; -----
label = s:!:Frontier:radio
sys   = PH_Frontier
sig   = 1:Host,User-Agent=[FSL IR/],?Cache-Control,Pragma=[no-cache]:Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:FSL IR/
sig   = 0:Host,User-Agent=[FSL IR/]:Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:FSL IR/

; ---
; Synology
; ---
label = s:!:Synology:
sys   = PH_Synology
sig   = 1:Accept=[*/*],Host=[checkip.synology.com]:User-Agent,Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:
sig   = 0:User-Agent=[synology_],Accept=[*/*],Host,Connection=[Keep-Alive]:Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:synology_

; ---
; Swann (security)
; ---
label = s:!:Swann:DVR
sys   = Nest
sig   = 1:Host=[mydvr.swanndvr.com],User-Agent,?Authorization,Connection=[close]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Wget

; --------------
; Minimal client
; --------------

label = s:!:Minimal:
sys   = @other
sig   = 0::Host,User-Agent,Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

; ---
; Kindle Classic
; ---

label = s:!:Kindle:Classic
sys   = Kindle
sig   = 1:?Accept-Language,?X-ADP-SW,User-Agent=[Java/phoneme_advanced],Host,Accept,Connection=[keep-alive]:Accept-Encoding,Accept-Charset,Keep-Alive:Java/phoneme_advanced-Core-1.3-b03

; ---
; Vizio
; ---

label = s:!:Vizio:
sys   = Linux
sig   = 1:Host,Accept=[*/*],User-Agent=[Vizio]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Vizio
; Too broad
; sig   = 1:User-Agent,Host,Accept=[*/*]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Mozilla/5.0

; ---
; Roku
; ---

label = s:!:Roku:
sys   = Linux
sig   = 1:Connection=[close],Host,User-Agent=[Roku]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Roku
sig   = 1:Host,User-Agent=[Roku]:Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Roku

; ---
; Boxee
; ---

label = s:!:Boxee:
sys   = Linux
sig   = 1:User-Agent,Host,Accept=[*/*],Accept-Encoding=[deflate, gzip],?Cookie,Accept-Charset=[ISO-8859-1,utf-8;q=0.7,*;q=0.7],Accept-Language=[en-us,en;q=0.5],Keep-Alive=[300],Connection=[keep-alive],X-Boxee-Client-Id::boxee
sig   = 1:User-Agent=[; Boxee/],?Cookie,Connection=[Keep-Alive],Accept-Encoding=[gzip],Accept-Language=[en,*],Host:Accept,Accept-Charset,Keep-Alive:; Boxee/

; ---
; Nest
; ---

label = s:!:Nest:
sys   = Nest
sig   = 1:User-Agent=[AddLightness],Host,Accept=[*/*]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:AddLightness

; ---
; A/V Equipment
; ---

label = s:!:Panasonic:
sys   = FreeBSD
sig   = 1:User-Agent=[U2FsdGVkX1],Host,Accept=[*/*],Accept-Charset=[utf-8]:Connection,Accept-Encoding,Accept-Language,Keep-Alive:U2FsdGVkX1
sig   = 1:User-Agent,Host=[vieraconnect.tv],Accept=[*/*],Pragma=[no-cache],?Cache-Control:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

label = s:!:Yamaha:AVR
sys   = Yamaha
sig   = 1:Host,User-Agent=[AV_Receiver/],Accept=[*/*],?Content-Length,Connection=[close]:Accept-Language,Accept-Charset,Keep-Alive:AV_Receiver/

label = s:!:Sony:TV
sys   = Sony
sig   = 1:Host,Accept=[*/*],X-WS-COUNTRY-CODE=[USA],X-WS-LANGUAGE-CODE,User-Agent:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:WidgetSystem/4.0.4
sig   = 1:User-Agent=[SONY DTV/],Host,Accept=[*/*]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:SONY DTV/
sig   = 1:Host,Accept=[*/*],?Cache-Control,User-Agent=[Sony BRAVIA]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Sony BRAVIA
sig   = 1:Host=[bravia-e.dl.playstation.net],Accept=[*/*]:User-Agent,Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

label = s:!:Sony:KDL-55HX850
sys   = Sony
sig   = 1:Host,Accept=[*/*],X-WS-MODEL-NAME=[KDL-55HX850],X-WS-CLIENT-ID,X-WS-COUNTRY-CODE,X-WS-LANGUAGE-CODE,User-Agent:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:WidgetSystem/4.0.4

label = s:!:Sony:KDL-55W900A
sys   = Sony
sig   = 1:Host,Accept=[*/*],?Cache-Control,User-Agent=[BRAVIA KDL-55W900A]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Sony BRAVIA KDL-55W900A

label = s:!:Sony:BD
sys   = Sony
sig   = 1:?Range,User-Agent=[SONY BD/],Host,Accept=[*/*]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:SONY BD/
sig   = 1:Host,Accept=[*/*],?Cookie,User-Agent=[SONY BD/]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:SONY BD/

label = s:!:Sony:Receiver
sys   = PH_NinSling
sig   = 1:Host,Connection=[close],User-Agent=[SONY HA-STR/],Accept=[*/*]:Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:SONY HA-STR/
sig   = 1:Host,Connection=[close],User-Agent=[L1.0#STR-],Accept=[*/*]:Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:L1.0#STR-

; Samsung over-the-network update
label = s:!:Samsung:OTN
sys   = Linux
sig   = 1:Host=[samsungotn.net],?If-Modified-Since,DUID:User-Agent,Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

label = s:!:Sharp:Aquos
sys   = PH_Arcadyan
sig   = 0:User-Agent=[AQUOS HTM_TIME],?Cache-Control,Host:Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:AQUOS HTM_TIME
sig   = 1:Host,User-Agent=[AQUOS_DL/],Pragma=[no-cache],Accept=[*/*]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:AQUOS_DL/

label = s:!:Sharp:LE830
sys   = PH_Arcadyan
sig   = 1:User-Agent=[(DEVTYPE=SH-TVLE830;],Content-Type,Connection=[Keep-Alive],Accept-Encoding=[gzip],Host:Accept,Accept-Charset,Keep-Alive:(DEVTYPE=SH-TVLE830;

; ---
; Symbian
; ---

label = s:!:Symbian:
sys   = Symbian
sig   = 1:Connection=[Keep-Alive],Host,Accept,Accept-Charset,Accept-Encoding=[gzip, deflate, x-gzip, identity; q=0.9],Accept-Language,?Cookie,?Cookie2,?Referer,User-Agent,X-Nokia-MusicShop-Bearer,PnP,X-Nokia-MusicShop-Version,x-wap-profile:Keep-Alive:SymbianOS/9

label = s:!:Symbian:E72
sys   = Symbian
sig   =  1:Connection=[Keep-Alive],Host,Accept,Accept-Charset,Accept-Encoding=[gzip, deflate, x-gzip, identity; q=0.9],Accept-Language,?Cookie,?Cookie2,User-Agent=[Mozilla/5.0 (SymbianOS/9.3; Series60/3.2 NokiaE72],PnP,X-Nokia-MusicShop-Bearer,X-Nokia-MusicShop-Version,x-wap-profile:Keep-Alive:Mozilla/5.0 (SymbianOS/9.3; Series60/3.2 NokiaE72

; ---
; Windows Phone
; ---

label = s:!:Nokia:Lumia_928
sys   = Windows
sig   = 1:User-Agent=[NOKIA; Lumia 928]::NOKIA; Lumia 928

label = s:!:Nokia:Lumia_920
sys   = Windows
sig   = 1:User-Agent=[NOKIA; Lumia 920]::NOKIA; Lumia 920

label = s:!:Nokia:Lumia_822
sys   = Windows
sig   = 1:User-Agent=[NOKIA; Lumia 822]::NOKIA; Lumia 822

label = s:!:Nokia:Lumia_521
sys   = Windows
sig   = 1:User-Agent=[NOKIA; Lumia 521]::NOKIA; Lumia 521

label = s:!:Nokia:Lumia_520
sys   = Windows
sig   = 1:User-Agent=[NOKIA; Lumia 520]::NOKIA; Lumia 520

; The semicolon in the UA is to prevent p0f from complaining about duplicate sigs
label = s:!:Nokia:Lumia
sys   = Windows
sig   = 1:User-Agent=[; NOKIA; Lumia]::; NOKIA; Lumia

label = s:!:WindowsPhone:
sys   = Windows
sig   = 1:Connection=[Keep-Alive],Accept=[*/*],If-Range,?Range,User-Agent=[; Windows Mobile],Unless-Modified-Since,Host:Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:; Windows Mobile
sig   = 1:Accept=[*/*],?Referer,Accept-Encoding=[identity],User-Agent=[NativeHost],Host,Connection=[Keep-Alive]:Accept-Language,Accept-Charset,Keep-Alive:NativeHost

; ---
; Netgear
; ---

label = s:!:MTK:NeoTV
sys   = Linux
sig   = 1:User-Agent=[Mozilla/5.0 (compatible; MTK/1.0)],Host,Accept=[*/*],?Referer,x-flash-version:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Mozilla/5.0 (compatible; MTK/1.0)

label = s:!:NeoTV:update
sys   = Linux
sig   = 1:Host=[updates1.netgear.com],Accept=[*/*]:User-Agent,Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

label = s:!:NeoTV:speedtest
sys   = Linux
sig   = 0:User-Agent=[HTTPing],Host=[ntvs.netgear.com]:Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:HTTPing

label = s:!:NeoTV:NTV300
sys   = Linux
sig   = 1:User-Agent=[NTV300],Host,Accept=[*/*],?Referer,x-flash-version:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:NTV300

label = s:!:NeoTV:GTV100
sys   = Linux
sig   = 1:Host,Connection=[keep-alive],User-Agent=[GTV100 Build],Accept-Encoding=[gzip,deflate,sdch],Accept-Language,Accept-Charset,?Cookie:Keep-Alive:GTV100 Build

; ---
; Others
; ---

label = s:!:Squeezebox:Server
sys   = Linux
sig   = 0:Connection=[close],?Cache-Control,Accept=[*/*],Accept-Encoding=[deflate, gzip],Accept-Language,Host,User-Agent=[Squeezebox Server, Logitech Media Server/],?Cookie,Cookie2,Icy-Metadata:Accept-Charset,Keep-Alive:Squeezebox Server, Logitech Media Server/
sig   = 0:?Cache-Control,Connection=[close],Accept=[*/*],Host,User-Agent=[SqueezeCenter, Squeezebox Server, Logitech Media Server/],Icy-Metadata=[1]:Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:SqueezeCenter, Squeezebox Server, Logitech Media Server/

label = s:!:Nook:
sys   = Linux
sig   = 1:Host,Connection=[Keep-Alive],User-Agent=[BN_USER_AGENT]:Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:BN_USER_AGENT

label = s:!:OregonScientific:
sys   = PH_Oregon
sig   = 0:Host=[static.oregonscientific.com],Connection=[close]:User-Agent,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

; This one is on a HomeWizard, and surely some other b0rked user agents
label = s:!:b0rked:
sys   = PH_HomeWizard
sig   = 1::Host,User-Agent,Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

label = s:!:Sonos:
sys   = Linux
sig   = 1:CONNECTION=[close],ACCEPT=[*/*],Host,USER-AGENT=[Sonos]:User-Agent,Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:
sig   = 1:CONNECTION=[close],ACCEPT,Host,USER-AGENT=[Sonos/],?If-None-Match:User-Agent,Connection,Accept,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

label = s:!:Seagate:BlackArmor
sys   = Linux
sig   = 1:User-Agent=[arm-mv-linux-gnu],Host=[www.seagate.com],Accept=[*/*]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:curl/7.18.2 (arm-mv-linux-gnu) libcurl/7.18.2 GnuTLS/2.4.2 zlib/1.2.3

label = s:!:Chromecast:Netflix
sys   = Linux
sig   = 1:Accept=[*/*],Host,?Range,X-Device=[2012.4 GOOGEUR001]:User-Agent,Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:

label = s:!:Chromecast:Mozilla
sys   = Linux
sig   =  1:Host,Connection=[keep-alive],Accept=[image/webp,*/*;q=0.8],User-Agent=[Mozilla/5.0 (CrKey],Accept-Encoding=[gzip,deflate],Accept-Language,?Cookie:Accept-Charset,Keep-Alive:Mozilla/5.0 (CrKey
sig   = 1:Host,Connection=[keep-alive],User-Agent=[CrKey],?Origin,Accept=[*/*],Accept-Encoding=[gzip,deflate],Accept-Language:Accept-Charset,Keep-Alive:CrKey
sig   = 1:Host,Connection=[keep-alive],Accept,User-Agent=[CrKey],Accept-Encoding=[gzip, deflate],Accept-Language,CAST-DEVICE-CAPABILITIES:Accept-Charset,Keep-Alive:CrKey
sig   = 1:Host,Connection=[keep-alive],User-Agent=[CrKey],?Origin,Accept=[*/*],Accept-Encoding=[gzip, deflate],Accept-Language,CAST-DEVICE-CAPABILITIES:Accept-Charset,Keep-Alive:CrKey

label = s:!:Debian:NetgearNAS
sys   = Linux
sig   = 1:Host=[readynas.com],?Cache-Control,Accept=[text/*],User-Agent=[Debian APT-HTTP]:Connection,Accept-Encoding,Accept-Language,Accept-Charset,Keep-Alive:Debian APT-HTTP/1.3 (1.0.9.2)

; ======================
; HTTP server signatures
; ======================

[http:response]
