#ifndef __ENDPOINT_SECURITY_CLIENT_H
#define __ENDPOINT_SECURITY_CLIENT_H

#ifndef __ENDPOINT_SECURITY_INDIRECT__
#error "Please #include <EndpointSecurity/EndpointSecurity.h> instead of this file directly."
#endif

#include <os/availability.h>
#include <os/base.h>

struct es_client_s;
/**
 * es_client_t is an opaque type that stores the endpoint security client state
 */
typedef struct es_client_s es_client_t;

__BEGIN_DECLS

/**
 * Subscribe to some set of events
 * @param client The client that will be subscribing
 * @param events Array of es_event_type_t to subscribe to
 * @param event_count Count of es_event_type_t in `events`
 * @return es_return_t indicating success or error
 * @note Subscribing to new event types does not remove previous subscriptions
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_subscribe(es_client_t * _Nonnull client, es_event_type_t * _Nonnull events, uint32_t event_count);

/**
 * Unsubscribe from some set of events
 * @param client The client that will be unsubscribing
 * @param events Array of es_event_type_t to unsubscribe from
 * @param event_count Count of es_event_type_t in `events`
 * @return es_return_t indicating success or error
 * @note Events not included in the given `events` array that were previously subscribed to will continue to be subscribed to
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_unsubscribe(es_client_t * _Nonnull client, es_event_type_t * _Nonnull events, uint32_t event_count);

/**
 * Unsubscribe from all events
 * @param client The client that will be unsubscribing
 * @return es_return_t indicating success or error
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_unsubscribe_all(es_client_t * _Nonnull client);

/**
 * List subscriptions
 * @param client The client for which subscriptions will be listed
 * @param count Out param that reports the number of subscriptions written
 * @param subscriptions  Out param for pointer to subscription data
 * @return es_return_t indicating success or error
 * @brief The caller takes ownership of the memory at `*subscriptions` and must free it
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_subscriptions(es_client_t * _Nonnull client, size_t * _Nonnull count,
		es_event_type_t * _Nonnull * _Nullable subscriptions);

/**
 * Respond to an auth event that requires an es_auth_result_t response
 * @param client The client that produced the event
 * @param message The message being responded to
 * @param result A result indicating the action the ES subsystem should take
 * @param cache Indicates if this result should be cached (the specific caching semantics depend on es_event_type_t)
 * @return es_respond_result_t indicating success or an error
 * @brief Some events must be responded to with `es_respond_flags_result`. Responding to flags events with this function will fail.
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_respond_result_t
es_respond_auth_result(es_client_t * _Nonnull client, const es_message_t * _Nonnull message, es_auth_result_t result, bool cache);

/**
 * Respond to an auth event that requires an uint32_t flags response
 * @param client The client that produced the event
 * @param message The message being responded to
 * @param authorized_flags A flags value that will mask the flags in event being responded to
 * @param cache Indicates if this result should be cached. The specific caching semantics depend on es_event_type_t
 * @return es_respond_result_t indicating success or an error
 * @brief Some events must be responded to with `es_respond_auth_result`. Responding to auth events with the function will fail.
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_respond_result_t
es_respond_flags_result(es_client_t * _Nonnull client, const es_message_t * _Nonnull message, uint32_t authorized_flags, bool cache);

/**
 * Suppress events relating to the process with `audit_token`
 * @param client The client for which events will be suppressed
 * @param audit_token The audit token of the process for which events will be suppressed
 * @return es_return_t indicating success or error
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_mute_process(es_client_t * _Nonnull client, const audit_token_t * _Nonnull audit_token);

/**
 * Unmute a process muted with es_mute_process
 * @param client The client for which the process will be unmuted
 * @param audit_token The audit token of the process to be unmuted
 * @return es_return_t indicating success or error
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_unmute_process(es_client_t * _Nonnull client, const audit_token_t *_Nonnull audit_token);

/**
 * List muted processes
 * @param client The client for which muted processes will be listed
 * @param count Out param that reports the number of audit tokens written
 * @param audit_tokens  Out param for pointer to audit_token data
 * @return es_return_t indicating success or error
 * @brief The caller takes ownership of the memory at `*audit_tokens` and must free it
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_muted_processes(es_client_t * _Nonnull client, size_t * _Nonnull count, audit_token_t * _Nonnull * _Nullable audit_tokens);

/**
 * Suppress events from executables matching a path prefix
 * @param client The client for which events will be suppressed
 * @param path_prefix The path against which supressed executables must prefix match
 * @return es_return_t indicating success or error
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_mute_path_prefix(es_client_t * _Nonnull client, const char * _Nonnull path_prefix);

/**
 * Suppress events from executables matching a path literal
 * @param client The client for which events will be suppressed
 * @param path_literal The path against which supressed executables must match exactly
 * @return es_return_t indicating success or error
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_mute_path_literal(es_client_t * _Nonnull client, const char * _Nonnull path_literal);

/**
 * Unmute all paths
 * @param client The client for which all currently muted paths will be unmuted
 * @return es_return_t indicating success or error
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_unmute_all_paths(es_client_t * _Nonnull client);

/**
 * Clear all cached results for all clients.
 * @param client that will perform the request
 * @return es_clear_cache_result_t value indicating success or an error
 * @discussion This functions clears the shared cache for all ES clients and is hence rate limited.
 *             If es_clear_cache is called too frequently it will return ES_CLEAR_CACHE_RESULT_ERR_THROTTLE
 *             It is permissible to pass any valid es_client_t object created by `es_new_client`
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_clear_cache_result_t
es_clear_cache(es_client_t * _Nonnull client);

/**
 * es_handler_block_t The type of block that will be invoked to handled messages from the ES subsystem
 * The es_client_t is a handle to the client being sent the event. It must be passed to any "respond" functions
 * The es_message_t is the message that must be handled
 */
typedef void (^es_handler_block_t)(es_client_t * _Nonnull, const es_message_t * _Nonnull);

/**
 * Initialise a new es_client_t and connect to the ES subsystem
 * @param client Out param. On success this will be set to point to the newly allocated es_client_t.
 * @param handler The handler block that will be run on all messages sent to this client
 * @return es_new_client_result_t indicating success or a specific error.
 * @discussion Messages are handled strictly serially and in the order they are delivered.
 *			   Returning control from the handler causes the next available message to be dequeued.
 *        	   Messages can be responded to out of order by returning control before calling es_respond_*.
 *			   The es_message_t is only guaranteed to live as long as the scope it is passed into.
 *			   The memory for the given es_message_t is NOT owned by clients and it must not be freed.
 *		  	   For out of order responding the handler must copy the message with es_copy_message.
 *		  	   Callers are required to be entitled with com.apple.developer.endpoint-security.client.
 *		  	   The application calling this interface must also be approved by users via Transparency, Consent & Control
 *		  	   (TCC) mechanisms using the Privacy Preferences pane and adding the application to Full Disk Access.
 *		  	   When a new client is successfully created, all cached results are automatically cleared.
 * @see es_copy_message
 * @see es_new_client_result_t
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_new_client_result_t
es_new_client(es_client_t * _Nullable * _Nonnull client, es_handler_block_t _Nonnull handler);

/**
 * Destroy an es_client_t, freeing resources and disconnecting from the ES subsystem
 * @param client The client to be destroyed
 * @return  ES_RETURN_SUCCESS indicates all resources were freed.
		   	ES_RETURN_ERROR indicates an error occured during shutdown and resources were leaked.
 */
OS_EXPORT
API_AVAILABLE(macos(10.15)) API_UNAVAILABLE(ios, tvos, watchos)
es_return_t
es_delete_client(es_client_t * _Nonnull client);

__END_DECLS;

#endif /* __ENDPOINT_SECURITY_CLIENT_H */
