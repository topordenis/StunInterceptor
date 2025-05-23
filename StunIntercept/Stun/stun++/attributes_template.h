// Copyright (c) 2014 Guilherme Balena Versiani.
//
// I dedicate any and all copyright interest in this software to the
// public domain. I make this dedication for the benefit of the public at
// large and to the detriment of my heirs and successors. I intend this
// dedication to be an overt act of relinquishment in perpetuity of all
// present and future rights to this software under copyright law.

// Note: this header is a header template and must NOT have multiple-inclusion
// protection.

// STUN attributes are all of the following types:
//
// - STUNXX_ATTRIBUTE_DATA(d, a)
// - STUNXX_ATTRIBUTE_EMPTY(d, a)
// - STUNXX_ATTRIBUTE_SOCKADDR(d, a)
// - STUNXX_ATTRIBUTE_XOR_SOCKADDR(d, a)
// - STUNXX_ATTRIBUTE_SPECIAL(d, a)
// - STUNXX_ATTRIBUTE_STRING(d, a)
// - STUNXX_ATTRIBUTE_UINT8(d, a)
// - STUNXX_ATTRIBUTE_UINT8_PAD(d, a)
// - STUNXX_ATTRIBUTE_UINT16(d, a)
// - STUNXX_ATTRIBUTE_UINT16_PAD(d, a)
// - STUNXX_ATTRIBUTE_UINT32(d, a)
// - STUNXX_ATTRIBUTE_UINT64(d, a)

// The following macro is handy when defining generic statements
#ifdef STUNXX_ATTRIBUTE_DEF
#define STUNXX_ATTRIBUTE_EMPTY(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_DATA(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_SOCKADDR(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_XOR_SOCKADDR(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_SPECIAL(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_STRING(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_UINT8(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_UINT8_PAD(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_UINT16(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_UINT16_PAD(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_UINT32(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#define STUNXX_ATTRIBUTE_UINT64(d, a) STUNXX_ATTRIBUTE_DEF(d, a)
#endif

// Define the several STUN attributes below:
STUNXX_ATTRIBUTE_SOCKADDR(STUN_ATTR_MAPPED_ADDRESS, mapped_address)
STUNXX_ATTRIBUTE_SOCKADDR(STUN_ATTR_RESPONSE_ADDRESS, response_address)
STUNXX_ATTRIBUTE_UINT32(STUN_ATTR_CHANGE_REQUEST, change_request)
STUNXX_ATTRIBUTE_SOCKADDR(STUN_ATTR_SOURCE_ADDRESS, source_address)
STUNXX_ATTRIBUTE_SOCKADDR(STUN_ATTR_CHANGED_ADDRESS, changed_address)
STUNXX_ATTRIBUTE_STRING(STUN_ATTR_USERNAME, username)
STUNXX_ATTRIBUTE_STRING(STUN_ATTR_PASSWORD, password)
STUNXX_ATTRIBUTE_SPECIAL(STUN_ATTR_MESSAGE_INTEGRITY, message_integrity)
STUNXX_ATTRIBUTE_SPECIAL(STUN_ATTR_ERROR_CODE, error_code)
STUNXX_ATTRIBUTE_SPECIAL(STUN_ATTR_UNKNOWN_ATTRIBUTES, unknown_attributes)
STUNXX_ATTRIBUTE_SOCKADDR(STUN_ATTR_REFLECTED_FROM, reflected_from)
STUNXX_ATTRIBUTE_UINT32(STUN_ATTR_CHANNEL_NUMBER, channel_number)
STUNXX_ATTRIBUTE_UINT32(STUN_ATTR_LIFETIME, lifetime)
STUNXX_ATTRIBUTE_UINT32(STUN_ATTR_BANDWIDTH, bandwidth)
STUNXX_ATTRIBUTE_XOR_SOCKADDR(STUN_ATTR_XOR_PEER_ADDRESS, xor_peer_address)
STUNXX_ATTRIBUTE_DATA(STUN_ATTR_DATA, data)
STUNXX_ATTRIBUTE_STRING(STUN_ATTR_REALM, realm)
STUNXX_ATTRIBUTE_STRING(STUN_ATTR_NONCE, nonce)
STUNXX_ATTRIBUTE_XOR_SOCKADDR(STUN_ATTR_XOR_RELAYED_ADDRESS, xor_relayed_address)
STUNXX_ATTRIBUTE_UINT8(STUN_ATTR_REQ_ADDRESS_FAMILY, req_address_family)
STUNXX_ATTRIBUTE_UINT8_PAD(STUN_ATTR_EVEN_PORT, even_port)
STUNXX_ATTRIBUTE_UINT32(STUN_ATTR_REQUESTED_TRANSPORT, requested_transport)
STUNXX_ATTRIBUTE_EMPTY(STUN_ATTR_DONT_FRAGMENT, dont_fragment)
STUNXX_ATTRIBUTE_XOR_SOCKADDR(STUN_ATTR_XOR_MAPPED_ADDRESS, xor_mapped_address)
STUNXX_ATTRIBUTE_UINT32(STUN_ATTR_TIMER_VAL, timer_val)
STUNXX_ATTRIBUTE_UINT64(STUN_ATTR_RESERVATION_TOKEN, reservation_token)
STUNXX_ATTRIBUTE_UINT32(STUN_ATTR_PRIORITY, priority)
STUNXX_ATTRIBUTE_EMPTY(STUN_ATTR_USE_CANDIDATE, use_candidate)
STUNXX_ATTRIBUTE_DATA(STUN_ATTR_PADDING, padding)
STUNXX_ATTRIBUTE_UINT16_PAD(STUN_ATTR_RESPONSE_PORT, response_port)
STUNXX_ATTRIBUTE_UINT32(STUN_ATTR_CONNECTION_ID, connection_id)
STUNXX_ATTRIBUTE_STRING(STUN_ATTR_SOFTWARE, software)
STUNXX_ATTRIBUTE_SOCKADDR(STUN_ATTR_ALTERNATE_SERVER, alternate_server)
STUNXX_ATTRIBUTE_SPECIAL(STUN_ATTR_FINGERPRINT, fingerprint)
STUNXX_ATTRIBUTE_UINT64(STUN_ATTR_ICE_CONTROLLED, ice_controlled)
STUNXX_ATTRIBUTE_UINT64(STUN_ATTR_ICE_CONTROLLING, ice_controlling)
STUNXX_ATTRIBUTE_SOCKADDR(STUN_ATTR_RESPONSE_ORIGIN, response_origin)
STUNXX_ATTRIBUTE_SOCKADDR(STUN_ATTR_OTHER_ADDRESS, other_address)

#ifdef STUNXX_ATTRIBUTE_DEF
#undef STUNXX_ATTRIBUTE_EMPTY
#undef STUNXX_ATTRIBUTE_DATA
#undef STUNXX_ATTRIBUTE_SOCKADDR
#undef STUNXX_ATTRIBUTE_XOR_SOCKADDR
#undef STUNXX_ATTRIBUTE_SPECIAL
#undef STUNXX_ATTRIBUTE_STRING
#undef STUNXX_ATTRIBUTE_UINT8
#undef STUNXX_ATTRIBUTE_UINT8_PAD
#undef STUNXX_ATTRIBUTE_UINT16
#undef STUNXX_ATTRIBUTE_UINT16_PAD
#undef STUNXX_ATTRIBUTE_UINT32
#undef STUNXX_ATTRIBUTE_UINT64
#endif

