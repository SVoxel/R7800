#ifndef IFNAMSIZ
#define IFNAMSIZ	16
#endif
#define IF_NAME "ethwan"
#define SW_MAX_NR_PORTS 2

#define ATHSIOCDEVPRIVATE  0x89F0

/*
#define SW_API_PORT_OFFSET         10

#define SW_API_PT_IGMPS_EN         (10 + SW_API_PORT_OFFSET)
#define SW_API_PT_IGMPS_DIS        (11 + SW_API_PORT_OFFSET)

#define SW_API_FDB_OFFSET          200
#define SW_API_FDB_ADD             (0  + SW_API_FDB_OFFSET)
#define SW_API_FDB_DELALL          (1  + SW_API_FDB_OFFSET)
#define SW_API_FDB_DELDYN          (2  + SW_API_FDB_OFFSET)
#define SW_API_FDB_DELPORT         (3  + SW_API_FDB_OFFSET)
#define SW_API_FDB_DELMAC          (4  + SW_API_FDB_OFFSET)
#define SW_API_FDB_NEXT            (5  + SW_API_FDB_OFFSET)*/

extern int confFD; 

typedef enum {
    A_FALSE,
    A_TRUE           
}a_bool_t;

typedef uint8_t           a_uint8_t;    
typedef int8_t            a_int8_t;     
typedef uint16_t          a_uint16_t;   
typedef int16_t           a_int16_t;    
typedef uint32_t          a_uint32_t;   
typedef int32_t           a_int32_t;    
typedef uint64_t          a_uint64_t;   
typedef int64_t           a_int64_t;    

#if (SW_MAX_NR_PORTS <= 32)
typedef a_uint32_t fal_pbmp_t;
#else
typedef a_uint64_t fal_pbmp_t;
#endif

typedef enum {
    SW_OK              = 0,       /* Operation succeeded                 */
    SW_FAIL            = -1,      /* Operation failed                    */
    SW_BAD_VALUE       = -2,      /* Illegal value                       */
    SW_OUT_OF_RANGE    = -3,      /* Value is out of range               */
    SW_BAD_PARAM       = -4,      /* Illegal parameter(s)                */
    SW_BAD_PTR         = -5,      /* Illegal pointer value               */
    SW_BAD_LEN         = -6,      /* Wrong length                        */
    SW_BAD_STATE       = -7,      /* Wrong state of state machine        */
    SW_READ_ERROR      = -8,      /* Read operation failed               */
    SW_WRITE_ERROR     = -9,      /* Write operation failed              */
    SW_CREATE_ERROR    = -10,     /* Fail in creating an entry           */
    SW_DELETE_ERROR    = -11,     /* Fail in deleteing an entry          */  
    SW_NOT_FOUND       = -12,     /* Entry not found                     */
    SW_NO_CHANGE       = -13,     /* The parameter(s) is the same        */
    SW_NO_MORE         = -14,     /* No more entry found                 */
    SW_NO_SUCH         = -15,     /* No such entry                       */
    SW_ALREADY_EXIST   = -16,     /* Tried to create existing entry      */
    SW_FULL            = -17,     /* Table is full                       */
    SW_EMPTY           = -18,     /* Table is empty                      */
    SW_NOT_SUPPORTED   = -19,     /* This request is not support         */
    SW_NOT_IMPLEMENTED = -20,     /* This request is not implemented     */
    SW_NOT_INITIALIZED = -21,     /* The item is not initialized         */
    SW_BUSY            = -22,     /* Operation is still running          */
    SW_TIMEOUT         = -23,     /* Operation Time Out                  */
    SW_DISABLE         = -24,     /* Operation is disabled               */
    SW_NO_RESOURCE     = -25,     /* Resource not available (memory ...) */
    SW_INIT_ERROR      = -26,     /* Error occured while INIT process    */
    SW_NOT_READY       = -27,     /* The other side is not ready yet     */
    SW_OUT_OF_MEM      = -28,     /* Cpu memory allocation failed.       */
    SW_ABORTED         = -29      /* Operation has been aborted.         */
} sw_error_t;                    

typedef enum {
    FAL_MAC_FRWRD = 0,
    FAL_MAC_DROP,
    FAL_MAC_CPY_TO_CPU,
    FAL_MAC_RDT_TO_CPU
} fal_fdb_cmd_t;

typedef struct {
    a_uint8_t addr[6];
} fal_mac_addr_t;

typedef struct {
	fal_mac_addr_t hwaddr;
	int port_map;
	int sa_drop; 
} arl_struct_t;

typedef struct {
    fal_mac_addr_t mac;
    a_uint16_t vid;
    fal_fdb_cmd_t dacmd;
    fal_fdb_cmd_t sacmd;
    union {
        a_uint32_t id;
        fal_pbmp_t map;
    } port;
    a_bool_t portmap_en;
    a_bool_t static_en;
    a_bool_t leaky_en;
    a_bool_t mirror_en;
    a_bool_t da_pri_en;
    a_uint8_t da_queue;
} fal_fdb_entry_t;

int addMFdb(uint32 group, uint32 port_map);
int delMFdb(uint32 group);


