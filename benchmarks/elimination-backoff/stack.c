#include "stack.h"

Simple_Stack S;
void **location;
int *collision;

void StackOp (ThreadInfo * pInfo) {
    if (TryPerformStackOp (p) == FALSE)
	LesOP (p);
    return;
}

void LesOP (ThreadInfo * p) {
    while (1) {
	location[mypid] = p;
	pos = GetPosition (p);
	him = collision[pos];
	while (!CAS (&collision[pos], him, mypid))
	    him = collision[pos];
	if (him != EMPTY) {
	    q = location[him];
	    if (q != NULL && q->id == him && q->op != p->op) {
		if (CAS (&location[mypid], p, NULL)) {
		    if (TryCollision (p, q) == TRUE)
			return;
		    else
			goto stack;
		} else {
		    FinishCollision (p);
		    return;
		}
	    }
	    delay (p->spin);
	    if (!CAS (&location[mypid], p, NULL)) {
		FinishCollision (p);
		return;
	    }
	stack:
	    if (TryPerformStackOp (p) == TRUE)
		return;
	}
    }
}
    
bool TryPerformStackOp (ThreadInfo * p) {
    Cell *phead, *pnext;
    if (p->op == PUSH) {
	phead = S.ptop;
	p->cell.pnext = phead;
	if (CAS (&S.ptop, phead, &p->cell))
	    return TRUE;
	else
	    return FALSE;
    }
    if (p->op == POP) {
	phead = S.ptop;
	if (phead == NULL) {
	    p->cell = EMPTY;
	    return TRUE;
	}
	pnext = phead->pnext;
	if (CAS (&S.ptop, phead, pnext)) {
	    p->cell = *phead;
	    return TRUE;
	} else {
	    p->cell = EMPTY;
	    return FALSE;
	}
    }
}

void FinishCollision (ProcessInfo * p) {
    if (p->op == POP) {
	p->pcell = location[mypid]->pcell;
	location[mypid] = NULL;
    }
}

void TryCollision (ThreadInfo * p, ThreadInfo * q) {
    if (p->op == PUSH) {
	if (CAS (&location[him], q, p))
	    return TRUE;
	else
	    return FALSE;
    }
    if (p->op == POP) {
	if (CAS (&location[him], q, NULL)) {
	    p->cell = q->cell;
	    location[mypid] = NULL;
	    return TRUE;
	}
	else
	    return FALSE;
    }
}
