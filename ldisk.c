#include "filesystem.h"

void updateLdisk() {
    disk *follow, *current, *temp;
    follow = Ldisk;
    current = follow->next;

    while (current != NULL) {
        // should get fewer blocks alternating status
        if (follow->status == current->status) {
            // extend follow block, remove current (dont need it in memory)
            follow->end = current->end;
            follow->next = current->next;
            temp = current;
            current = current->next;
            free(temp);
        } else {
            // blocks have different statuses, keep both
            follow = current;
            current = current->next;
        }
    }
}

int getLdisk(){
    disk *follow, *current, *temp;
    current = Ldisk;
    follow = NULL;

    while(current->status == USED) {
        follow = current;
        if (current->next == NULL) {
            // disk is full
            printf("You are out of memory");
            return -1;
        }
        current = current->next;
    }

    if (current->begin == current->end) {
        temp = current;
        temp->status = USED;
    } else {
        temp = (disk*)malloc(sizeof(disk));
        temp->status = USED;
        temp->begin = current->begin;
        temp->end = current->begin;

        current->begin ++;
        temp->next = current;
    }

    if (follow == NULL) Ldisk = temp;
    else follow->next = temp;

    updateLdisk();
    return temp->begin;
}