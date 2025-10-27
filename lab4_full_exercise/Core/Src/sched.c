/*
 * sched.c
 *
 * Created on: Nov 25, 2024
 * Author: nguye
 *
 * ĐÃ SỬA LẠI LOGIC:
 * - SCH_Update() chỉ có nhiệm vụ giảm Delay của head và set cờ RunMe.
 * - SCH_Dispatch_Tasks() sẽ chạy task, xóa task khỏi head, và thêm lại
 * (nếu là task định kỳ).
 * - Logic này sửa lỗi "use after free" (treo hệ thống) của code cũ.
 */

#include "main.h"
#include "sched.h"
#include <stdlib.h> // Cần cho malloc/free

sTask *head = NULL;
sTask *tail = NULL;
int count = 0;
uint32_t id = 0;
SCH_Error_Code_t Error_code_G = ERROR_SCH_NO_ERROR;

void clear_sched(void)
{
    while (head != NULL)
    {
        sTask *temp = head;
        head = head->next;
        free(temp);
    }

    head = NULL;
    tail = NULL;
}

void SCH_Init(void)
{
    clear_sched();
    Error_code_G = 0;
    count = 0;
    id = 0;
}

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD, int TaskID) {
    // Tạo task mới
    sTask *new_task = (sTask *)malloc(sizeof(sTask));
    if (new_task == NULL) {
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
        return 0; // Trả về 0 (NO_TASK_ID) nếu lỗi
    }

    if (id > SCH_MAX_TASKS) {
        id = 0;  // reset TaskID
    }

    // Gán giá trị cho task mới
    new_task->pTask = pFunction;
    new_task->Delay = DELAY;
    new_task->Period = PERIOD;
    new_task->RunMe = 0;
    if (TaskID < 0)
    {
        new_task->TaskID = id++;
    } else {
        new_task->TaskID = TaskID;
    }
    new_task->next = NULL;
    new_task->prev = NULL;

    // Nếu danh sách rỗng
    if (head == NULL) {
        head = new_task;
        tail = new_task;
        count++;
        return new_task->TaskID;
    }

    // Duyệt danh sách để tìm vị trí chèn
    sTask *temp = head;
    while (temp != NULL && temp->Delay <= DELAY) {
        DELAY -= temp->Delay; // Giảm giá trị DELAY tương đối
        temp = temp->next;
    }

    // Chèn task mới vào trước temp
    new_task->Delay = DELAY; // Delay đã được điều chỉnh
    if (temp != NULL) {
        new_task->next = temp;
        new_task->prev = temp->prev;

        if (temp->prev != NULL) {
            temp->prev->next = new_task;
        } else {
            head = new_task; // Nếu chèn ở đầu danh sách
        }
        temp->prev = new_task;

        // update the delay of the next task
        temp->Delay -= DELAY;

    } else {
        // Nếu chèn ở cuối danh sách
        tail->next = new_task;
        new_task->prev = tail;
        tail = new_task;
    }

    count++;
    return new_task->TaskID;
}


uint8_t SCH_Delete_Task(uint32_t taskID)
{
    sTask *temp = head;

    // Tìm task dựa trên ID
    while (temp != NULL && temp->TaskID != taskID)
    {
        temp = temp->next;
    }

    // Nếu không tìm thấy task
    if (temp == NULL)
    {
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
        return 0; // Trả về 0 (lỗi)
    }

    // Task được tìm thấy, xử lý xóa khỏi danh sách liên kết
    if (temp->prev != NULL)
    {
        temp->prev->next = temp->next;
    }
    else
    {
        // Task là 'head', cập nhật 'head'
        head = temp->next;
    }

    if (temp->next != NULL)
    {
        temp->next->prev = temp->prev;
        // Cập nhật Delay cho task kế tiếp (quan trọng)
        temp->next->Delay += temp->Delay;
    }
    else
    {
        // Task là 'tail', cập nhật 'tail'
        tail = temp->prev;
    }

    free(temp); // Giải phóng bộ nhớ
    count--;
    return 1; // Trả về 1 (thành công)
}


/*
 * ====================================================================
 * SỬA LẠI HÀM SCH_Update
 * ====================================================================
 * Nhiệm vụ duy nhất: Giảm Delay của head. Nếu Delay == 0, set cờ RunMe.
 * Không tự ý reset Period hay xóa task.
 */
void SCH_Update(void)
{
    if (head != NULL) // Chỉ kiểm tra head
    {
        if (head->Delay > 0)
        {
            // Nếu chưa đến lúc chạy, giảm Delay
            head->Delay--;
        }

        if (head->Delay == 0)
        {
            // Đến lúc chạy, đặt cờ
            head->RunMe = 1;
        }
    }
}


/*
 * ====================================================================
 * SỬA LẠI HÀM SCH_Dispatch_Tasks
 * ====================================================================
 * Nhiệm vụ: Chạy task 'head' nếu cờ RunMe = 1.
 * Sau đó xóa task 'head' và thêm lại (nếu là task định kỳ).
 * Logic này an toàn, không bị "use after free".
 */
void SCH_Dispatch_Tasks(void)
{
    // Chỉ kiểm tra task đầu tiên (head)
    if (head != NULL && head->RunMe > 0)
    {
        // 1. Lưu lại thông tin của task 'head'
        void (*pFunction_to_run)(void) = head->pTask;
        uint32_t period_of_task = head->Period;
        uint32_t taskID_of_task = head->TaskID;

        // 2. XÓA task 'head' ra khỏi danh sách (Hàm này sẽ free(head))
        SCH_Delete_Task(taskID_of_task);

        // 3. CHẠY task
        pFunction_to_run();

        // 4. Nếu task này là định kỳ (Period > 0)
        if (period_of_task > 0)
        {
            // THÊM LẠI task vào danh sách với Delay mới = Period
            SCH_Add_Task(pFunction_to_run,
                         period_of_task,
                         period_of_task,
                         taskID_of_task);
        }
        // Nếu Period = 0 (task 1 lần), nó sẽ không được thêm lại.
    }
}


// Hàm này được giữ lại vì code cũ của bạn có dùng
// (mặc dù logic mới không cần)
void deep_copy(sTask *dest, sTask *src)
{
    dest->pTask = src->pTask;
    dest->Delay = src->Delay;
    dest->Period = src->Period;
    dest->TaskID = src->TaskID;
    dest->RunMe = src->RunMe; // Thêm RunMe cho đầy đủ
}
