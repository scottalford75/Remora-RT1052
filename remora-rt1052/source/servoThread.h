#ifndef SOURCE_SERVOTHREAD_H_
#define SOURCE_SERVOTHREAD_H_

#include "fsl_qtmr.h"

void configServoThread(void);
void updateServoThread(void);

void monitorEthernet(void);
void readInputs(void);
void setOutputs(void);

void updatePWM(void);
void setupPWM(float);


#endif /* SOURCE_SERVOTHREAD_H_ */
