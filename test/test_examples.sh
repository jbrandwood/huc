#!/bin/sh

result=0

test_file()
{
hash=`openssl sha1 $1`

if [ "$hash" = "$2" ] ; then
  echo File: $1 PASS
else
  echo File: $1 FAIL!
  result=1
fi
}

echo ''
echo Checking compiled example projects against known-good SHA1 values ...
echo ''

test_file ../examples/huc/acd/ac_test.iso \
 "SHA1(../examples/huc/acd/ac_test.iso)= 1f5ab6efa8bbab1e8c5395670e7283323042915d"

test_file ../examples/huc/overlay/overlay.iso \
 "SHA1(../examples/huc/overlay/overlay.iso)= 49c15e35b06b6825cb03bbd47e7c73081993b1fa"

test_file ../examples/huc/promotion/promotion.pce \
 "SHA1(../examples/huc/promotion/promotion.pce)= 7467d737fba6c4c64c954974e349a145c50a945d"

test_file ../examples/huc/scroll/scroll.pce \
 "SHA1(../examples/huc/scroll/scroll.pce)= 501a75535830b88e010f34fbf53d3534e996d26d"

test_file ../examples/huc/sgx/sgx_test.iso \
 "SHA1(../examples/huc/sgx/sgx_test.iso)= ccef5153b363a903d13d12f338d1fd901d3a488f"

test_file ../examples/huc/sgx/sgx_test.sgx \
 "SHA1(../examples/huc/sgx/sgx_test.sgx)= 6063823f26d3a0067db67de8ef7201ddaa7e24cd"

test_file ../examples/huc/shmup/shmup.iso \
 "SHA1(../examples/huc/shmup/shmup.iso)= caaf0a07b62e63e32da5b8805a06b92521ae27af"

test_file ../examples/huc/shmup/shmup.pce \
 "SHA1(../examples/huc/shmup/shmup.pce)= 09b90d330def0f3d4b29de1a3d5cd27afb5302bc"

exit $result
