#ifndef uidMRDCVLSC_uint256_HPP
#define uidMRDCVLSC_uint256_HPP

// the "uint128.hpp" & "uint256.hpp" was created on a rush so it's implementation
// is not well designed, organized and optimized, this file is a note...
// put here a optimized uint256 implementation in the future

/* PROBLEMS ON THE FIRST DESIGN OF uint128 & uint256

    - the endian of uint64 inside uint128 is not ideal because it represent the msq:lsq
      in the array as is; msq=data[0]:lsq=data[1].
        
      It should be lsq=data[0]:msq=data[1],
        
      Because of this, poly1305's function for generating the mac gained some extra
      operations (overhead), where the `uint128.swapHighLow()` is called just to correct this
      behaviour by swapping the value of msq and lsq with each other.

    - uint256 has the same problem as uint128, though it's not really giving the poly1305
    some extra operations 
*/

#endif