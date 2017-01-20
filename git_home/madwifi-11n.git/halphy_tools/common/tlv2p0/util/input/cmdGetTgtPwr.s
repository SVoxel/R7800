# cmd
CMD= getTgtPwr

# cmd parm
PARM_START:
UINT8:freq:1:u:0
UINT32:rateBit:1:u:0
UINT8:userMode:1:u:0
PARM_END:

# rsp
RSP= getTgtPwrRsp

# rsp parm
PARM_START:
UINT8:tgtPwr:1:u:0
PARM_END:


