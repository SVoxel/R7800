Firmware signature generation code is outside the driver and this is done through
fw_pack utility. There are multiple steps involved in generating the signed
files. 
1. First make sure you have the RSA private and public keys
2. Second make sure that you add the public key as a .x509 certificate
   and that is added to the .c files as hex array 
3. With openssl tool, you can compute the signature in below steps and generate
   signed files
   1. Check options of fw_pack as mentioned below, and generate a temporary file
      with header generated and attached to it
   2. Compute the signature as mentioned below, on top of the temporary file and
      then capture it to a temporary file
   3. Attach the signature at end as mentioned below.
   4. Copy the generated files to the firmware folder on the board again.
   5. When driver is loaded make sure fw_code_sign=1 is set.
Sample commands

#step-1, generate header
#./fw_pack -g -t -p 0x40 -o ${1}.xsig  -v 'CNSS.BL.3.0-00141-S-1' -i ${1}
#step2, genereate signature on temp file
#openssl dgst -sha256 -sign signing_key.priv ${1}.xsig  > ${1}.sig.tmp
#mv ${1} ${1}.old
#step-3, attach the signature to the file again
#./fw_pack -G  -i ${1}.xsig  -s ${1}.sig.tmp -o ${1}
#rm ${1}.xsig
#rm ${1}.sig.tmp
