#!/bin/sh

OPENSSL=$(which openssl)
KEY_FILE=uhttpd.key
CRT_FILE=uhttpd.crt

#generate a 1024 bit rsa private/public key without pass-phrase encrypt to used for tls
${OPENSSL} genrsa -out ${KEY_FILE} 1024
#generate csr
${OPENSSL} req -new -x509 -key ${KEY_FILE} -out ${CRT_FILE} -days 3650 -subj "/C=US/ST=California/L=San Jose/O=NETGEAR/OU=Home Consumer Products/CN=www.routerlogin.net/emailAddress=support@netgear.com"
