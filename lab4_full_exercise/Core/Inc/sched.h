#ifndef INC_SCHED_H_
#define INC_SCHED_H_

#define SCH_MAX_TASKS 40
#define NO_TASK_ID 0

#include "main.h"
#include <stdlib.h> // Thêm thư viện này để dùng NULL và malloc/free

typedef enum {
    ERROR_SCH_NO_ERROR = 0,
    ERROR_SCH_TOO_MANY_TASKS = 1,
    ERROR_SCH_CANNOT_DELETE_TASK = 2,
    ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK = 3,
    ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER = 4,
    ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START = 5,
    ERROR_SCH_LOST_SLAVE = 6,
    ERROR_SCH_CAN_BUS_ERROR = 7,
    ERROR_I2C_WRITE_BYTE_AT24C64 = 8
} SCH_Error_Code_t;

typedef struct sTask{
    void (* pTask) (void);
    uint32_t Delay;
    uint32_t Period;
    uint8_t RunMe;
    uint32_t TaskID;
    struct sTask *next;
    struct sTask *prev;
} sTask;

extern sTask *head;
extern sTask *tail;
extern int count;
// Sửa 'int id' thành 'uint32_t id' cho nhất quán với kiểu trả về của Add_Task
extern uint32_t id;
extern SCH_Error_Code_t Error_code_G;


void clear_sched(void);
void SCH_Init(void);

// Sửa lại định nghĩa hàm (signature) cho đúng yêu cầu đề bài [cite: 680]
uint32_t SCH_Add_Task(void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD, int TaskID);

// Sửa lại định nghĩa hàm (signature) cho đúng yêu cầu đề bài [cite: 682]
uint8_t SCH_Delete_Task(uint32_t taskID);

void SCH_Dispatch_Tasks(void);
void SCH_Update(void);

void deep_copy(sTask *dest, sTask *src);

#endif /* INC_SCHED_H_ */
