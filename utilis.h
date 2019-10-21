#ifndef __UTILS_H__
#define	__UTILS_H__

#define PASS              0
#define FAIL              1

/* Size of data array */
#define ARRAY_SIZE        64U

/* Reference values in Q1.31 format */
#define DELTA             (int32_t)0x00001000       // Max residual error for sines, with 6 cycle precision:           
						
/* Number of calculation loops (depends on clock config) */
#define LOOP_NB           (uint32_t)((170000000 / 750) / ARRAY_SIZE)

#ifdef __cplusplus
extern "C" {
#endif
	
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t t);
	
void fpu_perfmance_test(void);
void crc_perfmance_test(void);

uint32_t crc32_algorithm_1(uint32_t * crc32, uint32_t ** const pp_src, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif	// __UTILS_H__
