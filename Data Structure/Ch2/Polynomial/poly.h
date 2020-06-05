#pragma once
#include "basic.h"

typedef struct _poly {
	float coef;
	int expon;
	struct _poly* link;
} poly;

poly* Zero();
int IsZero(poly* p);
int Coef(poly* p, int expon);
int LeadExp(poly* p);
poly* Attach(poly* p, float coef, int expon);
poly* Remove(poly** p, int expon);
poly* SingleMult(poly* p, float coef, int expon);
poly* Add(poly* p1, poly* p2);
poly* Mult(poly* p1, poly* p2);

poly* Zero() {
	return NULL;
}

int IsZero(poly* p) {
	if (!p)
		return 1;

	return 0;
}

int Coef(poly* p, int expon) {
	poly* temp = p;
	while (temp) {
		if (temp->expon == expon)
			return temp->coef;

		temp = temp->link;
	}

	return 0;
}

int LeadExp(poly* p) {
	poly* temp = p;
	if (IsZero(p))
		return 0;

	int max = temp->expon;
	temp = temp->link;

	while (temp) {
		if (temp->expon > max)
			max = temp->expon;

		temp = temp->link;
	}

	return max;
}

poly* Attach(poly* p, float coef, int expon) {
	if (!coef)
		return p;

	poly* _new = (poly*)calloc(sizeof(poly), 1);
	_new->coef = coef;
	_new->expon = expon;
	_new->link = NULL;

	if (IsZero(p))
		return _new;

	poly* top = p;
	if (top->expon < expon) {
		_new->link = top;
		top = _new;
		return top;
	}

	poly* cur = p;
	while (1) {
		if (cur->expon == expon) {
			free(_new);
			return top;
		}

		if (cur->link == NULL)
			break;

		if (cur->link->expon < expon)
			break;

		cur = cur->link;
	}

	_new->link = cur->link;
	cur->link = _new;
	return top;
}

poly* Remove(poly** p, int expon) {
	if (IsZero(*p))
		return NULL;

	poly* temp;

	if ((*p)->expon == expon) {
		temp = *p;
		*p = (*p)->link;
		return temp;
	}

	poly* cur = (*p)->link;
	poly* pre = *p;

	while (cur) {
		if (cur->expon == expon) {
			pre->link = cur->link;
			return cur;
		}
		
		cur = cur->link;
		pre = pre->link;
	}
	
	return cur;
}

poly* SingleMult(poly* p, float coef, int expon) {
	if (!coef)
		return Zero();

	poly* _new = Zero();
	poly* temp = p;
	
	while (temp) {
		_new = Attach(_new, coef * temp->coef, expon + temp->expon);
		temp = temp->link;
	}

	return _new;
}

poly* Add(poly* p1, poly* p2) {
	poly* _new = Zero();
	poly* t1 = Zero(), * t2 = Zero();
	float coef;

	while (p1) {
		t1 = Attach(t1, p1->coef, p1->expon);
		p1 = p1->link;
	}

	while (p2) {
		t2 = Attach(t2, p2->coef, p2->expon);
		p2 = p2->link;
	}

	while (!IsZero(t1) && !IsZero(t2)) {
		switch (IntCompare(LeadExp(t1), LeadExp(t2))) {
		case -1:
			_new = Attach(_new, Coef(t2, LeadExp(t2)), LeadExp(t2));
			free(Remove(&t2, LeadExp(t2)));
			break;

		case 0:
			coef = Coef(t1, LeadExp(t1)) + Coef(t2, LeadExp(t2));
			if (coef)
				_new = Attach(_new, coef, LeadExp(t1));

			free(Remove(&t1, LeadExp(t1)));
			free(Remove(&t2, LeadExp(t2)));
			break;

		case 1:
			_new = Attach(_new, Coef(t1, LeadExp(t1)), LeadExp(t1));
			free(Remove(&t1, LeadExp(t1)));
			break;
		}
	}

	while (t1) {
		_new = Attach(_new, Coef(t1, LeadExp(t1)), LeadExp(t1));
		free(Remove(&t1, LeadExp(t1)));
	}

	while (t2) {
		_new = Attach(_new, Coef(t2, LeadExp(t2)), LeadExp(t2));
		free(Remove(&t2, LeadExp(t2)));
	}

	return _new;
}

poly* Mult(poly* p1, poly* p2) {
	if (IsZero(p1) || IsZero(p2))
		return Zero();

	poly* _new = Zero();
	while (p2) {
		_new = Add(SingleMult(p1, p2->coef, p2->expon), _new);
		p2 = p2->link;
	}

	return _new;
}