XMC4700F144K2048 Relax Kit test.

# ARMCC
## with --cpp11:
C++ Standard 201103

## without --cpp11:
C++ Standard 199711

# ARMCLANG
## default:
C++ Standard 199711

## C++98
C++ Standard 199711

## gnu++98
C++ Standard 199711

## C++03
C++ Standard 199711

## C++11
C++ Standard 201103

## gnu++11
C++ Standard 201103

## C++14
C++ Standard 201402


# FPU calculation of sine and cosine performance test

## with single precision FPU
Correct CMSIS DSP output[3-149 = 146]
Correct Single precision output[150-551 = 401]
Correct double precision output[551-6880 = 6329]

## without single precision FPU
Correct CMSIS DSP output[3-149 = 146]
Correct Single precision output[150-2159 = 2009]
Correct double precision output[2160-8018 = 5858]
