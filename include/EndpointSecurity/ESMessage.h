#ifndef __ENDPOINT_SECURITY_MESSAGE_H
#define __ENDPOINT_SECURITY_MESSAGE_H

#ifndef __ENDPOINT_SECURITY_INDIRECT__
#error "Please #include <EndpointSecurity/EndpointSecurity.h> instead of this file directly."
#endif

#include <mach/message.h>
#include <stdbool.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <os/availability.h>
#include <os/base.h>

#ifndef KERNEL
#include <sys/types.h>
#include <sys/acl.h>
#endif

/**
 * The EndpointSecurity subsystem is responsible for creating, populating and
 * delivering these data structures to ES clients.
 */

/**
 * @brief es_file_t provides the stat information and path to a file that relates to a security
 * event. The path may be truncated, which is indicated by the path_truncated flag.
 *
 * @discussion Currently, path lengths are supported up to 16K in length (though this
 * number is subject to change at any time and users must not rely on this value being
 * constant). If a path is longer than the supported maximum length, it will be marked
 * as truncated via the `path_truncated` member.
 */
typedef struct {
	es_string_token_t path;
	bool path_truncated;
	struct stat64 stat;
} es_file_t;

/**
 * @brief Information related to a process. This is used both for describing processes that
 * instigated an event (e.g. in the case of the `es_message_t` `process` field, or are targets
 * of an event (e.g. for exec events this describes the new process being executed, for signal
 * events this describes the process that will receive the signal).
 *
 * @discussion
 * - Values such as PID, UID, GID, etc. can be extracted from the audit token via API in libbsm.h.
 * - Clients should take caution when processing events where `is_es_client` is true. If multiple ES
 *   clients exist, actions taken by one client could trigger additional actions by the other client,
 *   causing a potentially infinite cycle.
 */
typedef struct {
	audit_token_t audit_token;
	pid_t ppid; //Note: This field tracks the current parent pid
	pid_t original_ppid; //Note: This field stays constant even in the event a process is reparented
	pid_t group_id;
	pid_t session_id;
	uint32_t codesigning_flags; //Note: The values for these flags can be found in the include file `cs_blobs.h` (`#include <kern/cs_blobs.h>`)
	bool is_platform_binary;
	bool is_es_client; //indicates this process is connected to the ES subsystem.
	uint8_t cdhash[CS_CDHASH_LEN];
	es_string_token_t signing_id;
	es_string_token_t team_id;
	es_file_t * _Nullable executable;
} es_process_t;


#ifdef KERNEL
	typedef struct statfs64 es_statfs_t;
#else
	#if !__DARWIN_64_BIT_INO_T
		#error This header requires __DARWIN_64_BIT_INO_T
	#endif
	typedef struct statfs es_statfs_t;
#endif

/**
 * @brief Execute a new process
 *
 * @field target The new process that is being executed
 * @field args Contains the executable and environment arguments (see note)
 *
 * @note Process arguments and environment variables are packed, use the following
 * API functions to operate on this field:
 * `es_exec_env`, `es_exec_arg`, `es_exec_env_count`, and `es_exec_arg_count`
 */
typedef struct {
	es_process_t * _Nullable target;
	es_token_t args;
	uint8_t reserved[64];
} es_event_exec_t;

/**
 * @brief Open a file system object
 *
 * @field fflag The desired flags to be used when opening `file` (see note)
 * @field file The file that will be opened
 *
 * @note: The `fflag` field represents the mask as applied by the kernel, not as represented by typical
 * open(2) `oflag` values. When responding to `ES_EVENT_TYPE_AUTH_OPEN` events using
 * es_respond_flags_result(), ensure that the same FFLAG values are used (e.g. FREAD, FWRITE instead
 * of O_RDONLY, O_RDWR, etc...).
 *
 * @see fcntl.h
 */
typedef struct {
	int32_t fflag;
	es_file_t * _Nullable file;
	uint8_t reserved[64];
} es_event_open_t;

/**
 * @brief Load a kernel extension
 *
 * @field identifier The signing identifier of the kext being loaded
 */
typedef struct {
	es_string_token_t identifier;
	uint8_t reserved[64];
} es_event_kextload_t;

/**
 * @brief Unload a kernel extension
 *
 * @field identifier The signing identifier of the kext being unloaded
 */
typedef struct {
	es_string_token_t identifier;
	uint8_t reserved[64];
} es_event_kextunload_t;

/**
 * @brief Unlink a file system object
 *
 * @field target The object that will be removed
 * @field parent_dir The parent directory of the `target` file system object
 */
typedef struct {
	es_file_t * _Nullable target;
	es_file_t * _Nullable parent_dir;
	uint8_t reserved[64];
} es_event_unlink_t;

/**
 * @brief Memory map a file
 *
 * @field protection The protection (region accessibility) value
 * @field max_protection The maximum allowed protection value the operating system will respect
 * @field flags The type and attributes of the mapped file
 * @field file_pos The offset into `source` that will be mapped
 * @field source The file system object being mapped
 */
typedef struct {
	int32_t protection;
	int32_t max_protection;
	int32_t flags;
	uint64_t file_pos;
	es_file_t * _Nullable source;
	uint8_t reserved[64];
} es_event_mmap_t;

/**
 * @brief Link to a file
 *
 * @field source The existing object to which a hard link will be created
 * @field target_dir The directory in which the link will be created
 * @field target_filename The name of the new object linked to `source`
 */
typedef struct {
	es_file_t * _Nullable source;
	es_file_t * _Nullable target_dir;
	es_string_token_t target_filename;
	uint8_t reserved[64];
} es_event_link_t;

/**
 * @brief Mount a file system
 *
 * @field statfs The file system stats for the file system being mounted
 */
typedef struct {
	es_statfs_t * _Nullable statfs;
	uint8_t reserved[64];
} es_event_mount_t;

/**
 * @brief Unmount a file system
 *
 * @field statfs The file system stats for the file system being unmounted
 */
typedef struct {
	es_statfs_t * _Nullable statfs;
	uint8_t reserved[64];
} es_event_unmount_t;

/**
 * @brief Fork a new process
 *
 * @field child The child process that was created
 */
typedef struct {
	es_process_t * _Nullable child;
	uint8_t reserved[64];
} es_event_fork_t;

/**
 * @brief Control protection of pages
 *
 * @field protection The desired new protection value
 * @field address The base address to which the protection value will apply
 * @field size The size of the memory region the protection value will apply
 */
typedef struct {
	int32_t protection;
	user_addr_t address;
	user_size_t size;
	uint8_t reserved[64];
} es_event_mprotect_t;

/**
 * @brief Send a signal to a process
 *
 * @field sig The signal number to be delivered
 * @field target The process that will receive the signal
 *
 * @note This event will not fire if a process sends a signal to itself.
 */
typedef struct {
	int sig;
	es_process_t * _Nullable target;
	uint8_t reserved[64];
} es_event_signal_t;

typedef enum {
	ES_DESTINATION_TYPE_EXISTING_FILE,
	ES_DESTINATION_TYPE_NEW_PATH,
} es_destination_type_t;

/**
 * @brief Rename a file system object
 *
 * @field source The source file that is being renamed
 * @field destination_type Whether or not the destination refers to an existing or new file
 * @field destination Information about the destination of the renamed file (see note)
 * @field existing_file The destination file that will be overwritten
 * @field new_path Information regarding the destination of a newly created file
 * @field dir The directory into which the file will be renamed
 * @field filename The name of the new file that will be created
 *
 * @note The `destination_type` field describes which member in the `destination` union should
 * accessed. `ES_DESTINATION_TYPE_EXISTING_FILE` means that `existing_file` should be used,
 * `ES_DESTINATION_TYPE_NEW_PATH` means that the `new_path` struct should be used.
 */
typedef struct {
	es_file_t * _Nullable source;
	es_destination_type_t destination_type;
	union {
		es_file_t * _Nullable existing_file;
		struct {
			es_file_t * _Nullable dir;
			es_string_token_t filename;
		} new_path;
	} destination;
	uint8_t reserved[64];
} es_event_rename_t;

/**
 * @brief Set an extended attribute
 *
 * @field target The file for which the extended attribute will be set
 * @field extattr The extended attribute which will be set
 */
typedef struct {
	es_file_t * _Nullable target;
	es_string_token_t extattr;
	uint8_t reserved[64];
} es_event_setextattr_t;

/**
 * @brief Retrieve an extended attribute
 *
 * @field target The file for which the extended attribute will be retrieved
 * @field extattr The extended attribute which will be retrieved
 */
typedef struct {
	es_file_t * _Nullable target;
	es_string_token_t extattr;
	uint8_t reserved[64];
} es_event_getextattr_t;

/**
 * @brief Delete an extended attribute
 *
 * @field target The file for which the extended attribute will be deleted
 * @field extattr The extended attribute which will be deleted
 */
typedef struct {
	es_file_t * _Nullable target;
	es_string_token_t extattr;
	uint8_t reserved[64];
} es_event_deleteextattr_t;

/**
 * @brief Modify file mode
 *
 * @field mode The desired new mode
 * @field target The file for which mode information will be modified
 *
 * @note The `mode` member is the desired new mode. The `target`
 * member's `stat` information contains the current mode.
 */
typedef struct {
	mode_t mode;
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_setmode_t;

/**
 * @brief Modify file flags information
 *
 * @field flags The desired new flags
 * @field target The file for which flags information will be modified
 *
 * @note The `flags` member is the desired set of new flags. The `target`
 * member's `stat` information contains the current set of flags.
 */
typedef struct {
	uint32_t flags;
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_setflags_t;

/**
 * @brief Modify file owner information
 *
 * @field uid The desired new UID
 * @field gid The desired new GID
 * @field target The file for which owner information will be modified
 *
 * @note The `uid` and `gid` members are the desired new values. The `target`
 * member's `stat` information contains the current uid and gid values.
 */
typedef struct {
	uid_t uid;
	gid_t gid;
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_setowner_t;

/**
 * @brief Close a file descriptor
 *
 * @field modified Set to TRUE if the target file being closed has been modified
 * @field target The file that is being closed
 */
typedef struct {
	bool modified;
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_close_t;

/**
 * @brief Create a file system object
 *
 * @field destination_type Whether or not the destination refers to an existing file (see note)
 * @field destination Information about the destination of the new file (see note)
 * @field existing_file The file system object that was created
 * @field dir The directory in which the new file system object will be created
 * @field filename The name of the new file system object to create
 *
 * @note If an object is being created but has not yet been created, the
 * `destination_type` will be `ES_DESTINATION_TYPE_NEW_PATH`.
 *
 * @note Typically `ES_EVENT_TYPE_NOTIFY_CREATE` events are fired after the
 * object has been created and the `destination_type` will be
 * `ES_DESTINATION_TYPE_EXISTING_FILE`. The exception to this is for
 * notifications that occur if an ES client responds to an
 * `ES_EVENT_TYPE_AUTH_CREATE` event with `ES_AUTH_RESULT_DENY`.
 */
typedef struct {
	es_destination_type_t destination_type;
	union {
		es_file_t * _Nullable existing_file;
		struct {
			es_file_t * _Nullable dir;
			es_string_token_t filename;
			mode_t mode;
		} new_path;
	} destination;
	uint8_t reserved[64];
} es_event_create_t;

/**
 * @brief Terminate a process
 *
 * @field stat The exit status of a process (same format as wait(2))
 */
typedef struct {
	int stat;
	uint8_t reserved[64];
} es_event_exit_t;

/**
 * @brief Exchange data atomically between two files
 *
 * @field file1 The first file to be exchanged
 * @field file2 The second file to be exchanged
 */
typedef struct {
	es_file_t * _Nullable file1;
	es_file_t * _Nullable file2;
	uint8_t reserved[64];
} es_event_exchangedata_t;

/**
 * @brief Write to a file
 *
 * @field target The file being written to
 */
typedef struct {
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_write_t;

/**
 * @brief Truncate a file
 *
 * @field target The file that is being truncated
 */
typedef struct {
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_truncate_t;

/**
 * @brief Change directories
 *
 * @field target The desired new current working directory
 */
typedef struct {
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_chdir_t;

/**
 * @brief View stat information of a file
 *
 * @field target The file for which stat information will be retrieved
 */
typedef struct {
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_stat_t;

/**
 * @brief Change the root directory for a process
 *
 * @field target The directory which will be the new root
 */
typedef struct {
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_chroot_t;

/**
 * @brief List extended attributes of a file
 *
 * @field target The file for which extended attributes are being retrieved
 */
typedef struct {
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_listextattr_t;

/**
 * @brief Open an I/O Kit device
 *
 * @field user_client_type User client type
 * @field user_client_class Meta class name of the user client instance
 */
typedef struct {
	uint32_t user_client_type;
	es_string_token_t user_client_class;
	uint8_t reserved[64];
} es_event_iokit_open_t;

/**
 * @brief Get a process's task port
 *
 * @field target The process for which the task port will be retrieved
 */
typedef struct {
	es_process_t * _Nullable target;
	uint8_t reserved[64];
} es_event_get_task_t;

/**
 * @brief Retrieve file system attributes
 *
 * @field attrlist The attributes that will be retrieved
 * @field target The file for which attributes will be retrieved
 */
typedef struct {
	struct attrlist attrlist;
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_getattrlist_t;

/**
 * @brief Set file system attributes
 *
 * @field attrlist The attributes that will be modified
 * @field target The file for which attributes will be modified
 */
typedef struct {
	struct attrlist attrlist;
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_setattrlist_t;

/**
 * @brief Update file contents via the FileProvider framework
 *
 * @field source The staged file that has had its contents updated
 * @field target_path The destination that the staged `source` file will be moved to
 */
typedef struct {
	es_file_t * _Nullable source;
	es_string_token_t target_path;
	uint8_t reserved[64];
} es_event_file_provider_update_t;

/**
 * @brief Materialize a file via the FileProvider framework
 *
 * @field source The staged file that has been materialized
 * @field target The destination of the staged `source` file
 */
typedef struct {
	es_process_t * _Nullable instigator;
	es_file_t * _Nullable source;
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_file_provider_materialize_t;

/**
 * @brief Resolve a symbolic link
 *
 * @field source The symbolic link that is attempting to be resolved
 *
 * @note This is not limited only to readlink(2). Other operations such as path lookups
 * can also cause this event to be fired.
 */
typedef struct {
	es_file_t * _Nullable source;
	uint8_t reserved[64];
} es_event_readlink_t;

/**
 * @brief Lookup a file system object
 *
 * @field source_dir The current directory
 * @field relative_target The path to lookup relative to the `source_dir`
 *
 * @note The `relative_target` data may contain untrusted user input.
 */
typedef struct {
	es_file_t * _Nullable source_dir;
	es_string_token_t relative_target;
	uint8_t reserved[64];
} es_event_lookup_t;

/**
 * @brief Test file access
 *
 * @field mode Access permission to check
 * @field target The file to check for access
 */
typedef struct {
	int32_t mode;
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_access_t;

/**
 * @brief Change file access and modification times (e.g. via utimes(2))
 *
 * @field target The path which will have its times modified
 * @field atime The desired new access time
 * @field mtime The desired new modification time
 */
typedef struct {
	es_file_t * _Nullable target;
	struct timespec atime;
	struct timespec mtime;
	uint8_t reserved[64];
} es_event_utimes_t;

/**
 * @brief Clone a file
 *
 * @field source The file that will be cloned
 * @field target_dir The directory into which the `source` file will be cloned
 * @field target_name The name of the new file to which `source` will be cloned
 */
typedef struct {
	es_file_t * _Nullable source;
	es_file_t * _Nullable target_dir;
	es_string_token_t target_name;
	uint8_t reserved[64];
} es_event_clone_t;

/**
 * @brief File control
 *
 * @field target The target file on which the file control command will be performed
 * @field cmd The `cmd` argument given to fcntl(2)
 */
typedef struct {
	es_file_t * _Nullable target;
	int32_t cmd;
	uint8_t reserved[64];
} es_event_fcntl_t;

/**
 * @brief Read directory entries
 *
 * @field target The directory whose contents will be read
 */
typedef struct {
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_readdir_t;

/**
 * @brief Retrieve file system path based on FSID
 *
 * @field target Describes the file system path that will be retrieved
 */
typedef struct {
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_fsgetpath_t;

/**
 * @brief Modify the system time
 *
 * @note This event is not fired if the program contains the entitlement
 * com.apple.private.settime. Additionally, even if an ES client responds to
 * ES_EVENT_TYPE_AUTH_SETTIME events with ES_AUTH_RESULT_ALLOW, the operation
 * may still fail for other reasons (e.g. unprivileged user).
 */
typedef struct {
	uint8_t reserved[64];
} es_event_settime_t;

/**
 * @brief Duplicate a file descriptor
 *
 * @field target Describes the file the duplicated file descriptor points to
 */
typedef struct {
	es_file_t * _Nullable target;
	uint8_t reserved[64];
} es_event_dup_t;

/**
 * @brief Fired when a UNIX-domain socket is about to be bound to a path.
 *
 * @field dir Describes the directory the socket file is created in.
 * @field filename The filename of the socket file.
 * @field mode The mode of the socket file.
 */
typedef struct {
	es_file_t * _Nullable dir;
	es_string_token_t filename;
	mode_t mode;
	uint8_t reserved[64];
} es_event_uipc_bind_t;

/**
 * @brief Fired when a UNIX-domain socket is about to be connected.
 *
 * @field file Describes the socket file that the socket is bound to.
 * @field domain The cmmunications domain of the socket (see socket(2)).
 * @field type The type of the socket (see socket(2)).
 * @field protocol The protocol of the socket (see socket(2)).
 */
typedef struct {
	es_file_t * _Nullable file;
	int domain;
	int type;
	int protocol;
	uint8_t reserved[64];
} es_event_uipc_connect_t;

/**
 * @brief Set a file ACL.
 *
 * @field set_or_clear Describes whether or not the ACL on the `target` is being set or cleared
 * @field acl Union that is valid when `set_or_clear` is set to `ES_SET`
 * @field set The acl_t structure to be used by vairous acl(3) functions
 * @field target Describes the file whose ACL is being set.
 */
typedef struct {
	es_file_t * _Nullable target;
	es_set_or_clear_t set_or_clear;
	union {
		acl_t _Nullable set;
	} acl;
	uint8_t reserved[64];
} es_event_setacl_t;

/**
 * Union of all possible events that can appear in an es_message_t
 */
typedef union {
	es_event_access_t access;
	es_event_close_t close;
	es_event_create_t create;
	es_event_deleteextattr_t deleteextattr;
	es_event_exchangedata_t exchangedata;
	es_event_exec_t exec;
	es_event_exit_t exit;
	es_event_file_provider_materialize_t file_provider_materialize;
	es_event_file_provider_update_t file_provider_update;
	es_event_fork_t fork;
	es_event_get_task_t get_task;
	es_event_getattrlist_t getattrlist;
	es_event_getextattr_t getextattr;
	es_event_iokit_open_t iokit_open;
	es_event_kextload_t kextload;
	es_event_kextunload_t kextunload;
	es_event_link_t link;
	es_event_listextattr_t listextattr;
	es_event_lookup_t lookup;
	es_event_mmap_t mmap;
	es_event_mount_t mount;
	es_event_mprotect_t mprotect;
	es_event_open_t open;
	es_event_readdir_t readdir;
	es_event_readlink_t readlink;
	es_event_rename_t rename;
	es_event_setacl_t setacl;
	es_event_setattrlist_t setattrlist;
	es_event_setextattr_t setextattr;
	es_event_setflags_t setflags;
	es_event_setmode_t setmode;
	es_event_setowner_t setowner;
	es_event_settime_t settime;
	es_event_signal_t signal;
	es_event_stat_t stat;
	es_event_truncate_t truncate;
	es_event_unlink_t unlink;
	es_event_unmount_t unmount;
	es_event_write_t write;
	es_event_chdir_t chdir;
	es_event_chroot_t chroot;
	es_event_utimes_t utimes;
	es_event_clone_t clone;
	es_event_fcntl_t fcntl;
	es_event_fsgetpath_t fsgetpath;
	es_event_dup_t dup;
	es_event_uipc_bind_t uipc_bind;
	es_event_uipc_connect_t uipc_connect;
} es_events_t;

/**
 * es_result_t indicates the result of the ES subsystem authorization process
 * The result_type field indicates if the result is an es_auth_result_t or a uint32_t (flags)
 */
typedef struct {
	es_result_type_t result_type;
	union {
		es_auth_result_t auth;
		uint32_t flags;
		uint8_t reserved[32];
	} result;
} es_result_t;

/**
 * es_message_t is the top level datatype that encodes information sent from the ES subsystem to its clients
 * Each security event being processed by the ES subsystem will be encoded in an es_message_t
 * A message can be an authorization request or a notification of an event that has already taken place
 * The action_type indicates if the action field is an auth or notify action
 * The event_type indicates which event struct is defined in the event union.
 * For events that can be authorized there are unique NOTIFY and AUTH event types for the same event data
 * eg: event.exec is the correct union label for both ES_EVENT_TYPE_AUTH_EXEC and ES_EVENT_TYPE_NOTIFY_EXEC event types
 */
typedef struct {
	uint32_t version;
	struct timespec time;
	uint64_t mach_time;
	uint64_t deadline;
	es_process_t * _Nullable process;
	uint8_t reserved[8];
	es_action_type_t action_type;
	union {
		es_event_id_t auth;
		es_result_t notify;
	} action;
	es_event_type_t event_type;
	es_events_t event;
	uint64_t opaque[]; /* Opaque data that must not be accessed directly */
} es_message_t;

__BEGIN_DECLS

/**
 * Calculate the size of an es_message_t.
 * @param msg The message for which the size will be calculated
 * @return Size of the message
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
size_t
es_message_size(const es_message_t * _Nonnull msg);

/**
 * Copy an es_message_t, allocating new memory.
 * @param msg The message to be copied
 * @return pointer to newly allocated es_message_t.
 *
 * @brief The caller owns the memory for the returned es_message_t and must free it using es_free_message.
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_message_t * _Nullable
es_copy_message(const es_message_t * _Nonnull msg);

/**
 * Frees the memory associated with the given es_message_t
 * @param msg The message to be freed
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
void
es_free_message(es_message_t * _Nonnull msg);

/**
 * Get the number of arguments in a message containing an es_event_exec_t
 * @param event The es_exec_event_t being inspected
 * @return The number of arguments
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
uint32_t
es_exec_arg_count(const es_event_exec_t * _Nonnull event);

/**
 * Get the number of environment variables in a message containing an es_event_exec_t
 * @param event The es_exec_event_t being inspected
 * @return The number of environment variables
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
uint32_t
es_exec_env_count(const es_event_exec_t * _Nonnull event);

/**
 * Get the argument at the specified position in the message containing an es_event_exec_t
 * @param event The es_exec_event_t being inspected
 * @param index Index of the argument to retrieve (starts from 0)
 * @return  es_string_token_t containing a pointer to the argument and its length.
 *          This is a zero-allocation operation. The returned pointer must not outlive exec_event.
 * @brief Reading an an argument where `index` >= `es_exec_arg_count()` is undefined
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_string_token_t
es_exec_arg(const es_event_exec_t * _Nonnull event, uint32_t index);

/**
 * Get the environment variable at the specified position in the message containing an es_event_exec_t
 * @param event The es_exec_event_t being inspected
 * @param index Index of the environment variable to retrieve (starts from 0)
 * @return  es_string_token_t containing a pointer to the environment variable and its length.
 *          This is zero-allocation operation. The returned pointer must not outlive exec_event.
 * @brief Reading an an env where `index` >= `es_exec_env_count()` is undefined
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_string_token_t
es_exec_env(const es_event_exec_t * _Nonnull event, uint32_t index);

__END_DECLS

#endif /* __ENDPOINT_SECURITY_MESSAGE_H */
