/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"

#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IEcoLab1
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ IEcoLab1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
	else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)){
		if (pCMe->m_pInnerUnknown != 0) {
			return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(
				pCMe->m_pInnerUnknown, riid, ppv);
		}
		*ppv = &pCMe->m_pVTblIEcoCalculatorX;
		if (ppv == 0 || pCMe->m_pIEcoCalculatorX == 0) {
			return ERR_ECO_NOINTERFACE;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
	}
	else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)){
		*ppv = &pCMe->m_pVTblIEcoCalculatorY;
		if (ppv == 0 || pCMe->m_pIEcoCalculatorY == 0) {
			return ERR_ECO_NOINTERFACE;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
	}
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoLab1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoLab1
 * </описание>
 *
 */
static uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    /* Проверка указателя */
    if (me == 0 ) {
        return -1; /* ERR_ECO_POINTER */
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ IEcoLab1Ptr_t me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Проверка указателей */
    if (me == 0 ) {
        return result;
    }

	/* Получение интерфейса IEcoCalculatorY (Multiplication и Division) у внутренних компонентов D, E (Включение) */
	result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIEcoCalculatorY);
	if (result != 0 || pCMe->m_pIEcoCalculatorY == 0) {
		result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIEcoCalculatorY);
	}

	/* Получение интерфейса IEcoCalculatorY (Addition и Subtraction) у внутреннего компонента B (Аггрегирование) */
	result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB,
		(IEcoUnknown*)pCMe, &IID_IEcoUnknown, (void**) &pCMe->m_pInnerUnknown);
	if (result != 0 || pCMe->m_pInnerUnknown == 0) {
		/* Получение интерфейса IEcoCalculatorY (Addition и Subtraction) у внутреннего компонента A (Включение) */
		result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIEcoCalculatorX);
	}

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return result;
}

/*
 *
 * <сводка>
 *   Функция QueryInterfaceX
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface, компонента IEcoCalculatorX
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_QueryInterfaceX(/* in */ struct IEcoCalculatorX* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));
    int16_t result = ERR_ECO_POINTER;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return result;
    }

    /* Проверка и получение запрошенного интерфейса */
    if (IsEqualUGUID(riid, &IID_IEcoLab1)) {
		*ppv = &pCMe->m_pVTblIEcoLab1;
		result = pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
	else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        if (pCMe->m_pInnerUnknown != 0) {
            // Запрашиваем интерфейс IEcoCalculatorX внутреннего компонента
            return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
        }
        else {
			*ppv = &pCMe->m_pVTblIEcoCalculatorX;
		}
		if (ppv == 0 || pCMe->m_pIEcoCalculatorX == 0) {
			return ERR_ECO_NOINTERFACE;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) ) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
		if (ppv == 0 || pCMe->m_pIEcoCalculatorY == 0) {
			return ERR_ECO_NOINTERFACE;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        if (pCMe->m_pInnerUnknown != 0) {
            // Запрашиваем интерфейс IEcoCalculatorX внутреннего компонента
            return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
        }
        else {
			*ppv = &pCMe->m_pVTblIEcoCalculatorX;
		}
		if (ppv == 0 || pCMe->m_pIEcoCalculatorX == 0) {
			return ERR_ECO_NOINTERFACE;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция AddRefX
 * </сводка>
 *
 * <описание>
 *   Функция AddRef, компонента IEcoCalculatorX
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_AddRefX(/* in */ struct IEcoCalculatorX* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция ReleaseX
 * </сводка>
 *
 * <описание>
 *   Функция Release, компонента IEcoCalculatorX
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_ReleaseX(/* in */ struct IEcoCalculatorX* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

	/* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        if ( pCMe->m_pInnerUnknown != 0 ) {
            // Предотвращение рекурсивного вызова
            if (pCMe->m_pInnerUnknown->pVTbl->Release(pCMe->m_pInnerUnknown) == 0) {
                pCMe->m_pInnerUnknown = 0;
            }
            else {
                pCMe->m_cRef = 1;
            }
        }
        if ( pCMe->m_cRef == 0 ) {
            deleteCEcoLab1((IEcoLab1*)pCMe);
        }
        return ERR_ECO_SUCCESES;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция AdditionX
 * </сводка>
 *
 * <описание>
 *   Функция сложения, компонента IEcoCalculatorX
 * </описание>
 *
 */
int32_t ECOCALLMETHOD CEcoLab1_AdditionX(/* in */ struct IEcoCalculatorX* me, /* in */ int16_t a, /* in */ int16_t b) {
	CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));
    int32_t result = ERR_ECO_POINTER;

    /* Проверка указателей */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

	if (pCMe->m_pIEcoCalculatorX != 0) {
		result = pCMe->m_pIEcoCalculatorX->pVTbl->Addition(pCMe->m_pIEcoCalculatorX, a, b);
	}

    return result;
}

/*
 *
 * <сводка>
 *   Функция Subtraction
 * </сводка>
 *
 * <описание>
 *   Функция вычетания, компонента IEcoCalculatorX
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_SubtractionX(/* in */ struct IEcoCalculatorX* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoLab1*));
    int16_t result = ERR_ECO_POINTER;

    /* Проверка указателей */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pIEcoCalculatorX != 0) {
		result = pCMe->m_pIEcoCalculatorX->pVTbl->Subtraction(pCMe->m_pIEcoCalculatorX, a, b);
	}

    return result;
}

/*
 *
 * <сводка>
 *   Функция QueryInterfaceY
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface, компонента IEcoCalculatorY
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_QueryInterfaceY(/* in */ struct IEcoCalculatorY* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoCalculatorX*) - sizeof(struct IEcoLab1*));
    int16_t result = ERR_ECO_POINTER;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return result;
    }

    /* Проверка и получение запрошенного интерфейса */
    if ( IsEqualUGUID(riid, &IID_IEcoLab1) ) {
		*ppv = &pCMe->m_pVTblIEcoLab1;
		result = pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) ) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
		if (ppv == 0 || pCMe->m_pIEcoCalculatorY == 0) {
			return ERR_ECO_NOINTERFACE;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
	else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) ) {
        if (pCMe->m_pInnerUnknown != 0) {
            // Запрашиваем интерфейс IEcoCalculatorX внутреннего компонента
            return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
        }
        else {
			*ppv = &pCMe->m_pVTblIEcoCalculatorX;
		}
		if (ppv == 0 || pCMe->m_pIEcoCalculatorX == 0) {
			return ERR_ECO_NOINTERFACE;
		}
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
		pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция AddRefY
 * </сводка>
 *
 * <описание>
 *   Функция AddRef, компонента IEcoCalculatorY
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_AddRefY(/* in */ struct IEcoCalculatorY* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoCalculatorX*) - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция ReleaseY
 * </сводка>
 *
 * <описание>
 *   Функция Release, компонента IEcoCalculatorY
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_ReleaseY(/* in */ struct IEcoCalculatorY* me) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoCalculatorX*) - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

	/* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        if ( pCMe->m_pInnerUnknown != 0 ) {
            // Предотвращение рекурсивного вызова
            if (pCMe->m_pInnerUnknown->pVTbl->Release(pCMe->m_pInnerUnknown) == 0) {
                pCMe->m_pInnerUnknown = 0;
            }
            else {
                pCMe->m_cRef = 1;
            }
        }
        if ( pCMe->m_cRef == 0 ) {
            deleteCEcoLab1((IEcoLab1*)pCMe);
        }
        return ERR_ECO_SUCCESES;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция MultiplicationY
 * </сводка>
 *
 * <описание>
 *   Функция умножения, организованная механизмом включения компонента IEcoCalculatorY
 * </описание>
 *
 */
int32_t ECOCALLMETHOD CEcoLab1_MultiplicationY(/* in */ struct IEcoCalculatorY* me, /* in */ int16_t a, /* in */ int16_t b) {
	CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoCalculatorX*) - sizeof(struct IEcoLab1*));
    int32_t result = ERR_ECO_POINTER;

    /* Проверка указателей */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

	if (pCMe->m_pIEcoCalculatorY != 0) {
		result = pCMe->m_pIEcoCalculatorY->pVTbl->Multiplication(pCMe->m_pIEcoCalculatorY, a, b);
	}

    return result;
}

/*
 *
 * <сводка>
 *   Функция DivisionY
 * </сводка>
 *
 * <описание>
 *   Функция деления, организованная механизмом включения компонента IEcoCalculatorY
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_DivisionY(/* in */ struct IEcoCalculatorY* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*)((uint64_t)me - sizeof(struct IEcoCalculatorX*) - sizeof(struct IEcoLab1*));
    int16_t result = ERR_ECO_POINTER;

    /* Проверка указателей */
    if (me == 0 ) {
        return ERR_ECO_POINTER;
    }

    if (pCMe->m_pIEcoCalculatorY != 0) {
		result = pCMe->m_pIEcoCalculatorY->pVTbl->Division(pCMe->m_pIEcoCalculatorY, a, b);
	}

    return result;
}

/*
 *
 * <сводка>
 *   Функция doMergeSortInt
 * </сводка>
 *
 * <описание>
 *   Рекурсивная функция, выполняющая сортировку слиянием, с типом Int
 * </описание>
 *
 */
static void doMergeSortInt(CEcoLab1* pCMe, int* arr, int l, int r) {
	int midIdx, firstIdx, secondIdx;
	int curPartIndex = 0;
	int* curPart = 0;
	int swapValue;

	if (l >= r) {
		return;
	} else if (l + 1 == r) {
		if (arr[l] > arr[r]) {
			swapValue = arr[l];
			arr[l] = arr[r];
			arr[r] = swapValue;
		}
		return;
	}

	midIdx = (l + r) / 2;
	doMergeSortInt(pCMe, arr, l, midIdx);
	doMergeSortInt(pCMe, arr, midIdx + 1, r);

	firstIdx = l;
	secondIdx = midIdx + 1;
	curPartIndex = 0;
	curPart = (int*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(int) * (r - l + 1));

	while (firstIdx <= midIdx && secondIdx <= r) {
		if (arr[firstIdx] < arr[secondIdx]) {
			curPart[curPartIndex] = arr[firstIdx];
			curPartIndex++;
			firstIdx++;
		} else {
			curPart[curPartIndex] = arr[secondIdx];
			curPartIndex++;
			secondIdx++;
		}
	}

	while (secondIdx <= r) {
		curPart[curPartIndex] = arr[secondIdx];
		curPartIndex++;
		secondIdx++;
	}
	while (firstIdx <= midIdx) {
		curPart[curPartIndex] = arr[firstIdx];
		curPartIndex++;
		firstIdx++;
	}

	for (curPartIndex = l; curPartIndex <= r; ++curPartIndex) {
		arr[curPartIndex] = curPart[curPartIndex - l];
	}
	
	pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, curPart);
}

/*
 *
 * <сводка>
 *   Функция doMergeSortLong
 * </сводка>
 *
 * <описание>
 *   Рекурсивная функция, выполняющая сортировку слиянием, с типом Long
 * </описание>
 *
 */
static void doMergeSortLong(CEcoLab1* pCMe, long* arr, int l, int r) {
	int midIdx, firstIdx, secondIdx;
	int curPartIndex = 0;
	long* curPart = 0;
	long swapValue;

	if (l >= r) {
		return;
	} else if (l + 1 == r) {
		if (arr[l] > arr[r]) {
			swapValue = arr[l];
			arr[l] = arr[r];
			arr[r] = swapValue;
		}
		return;
	}

	midIdx = (l + r) / 2;
	doMergeSortLong(pCMe, arr, l, midIdx);
	doMergeSortLong(pCMe, arr, midIdx + 1, r);

	firstIdx = l;
	secondIdx = midIdx + 1;
	curPartIndex = 0;
	curPart = (long*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(long) * (r - l + 1));

	while (firstIdx <= midIdx && secondIdx <= r) {
		if (arr[firstIdx] < arr[secondIdx]) {
			curPart[curPartIndex] = arr[firstIdx];
			curPartIndex++;
			firstIdx++;
		} else {
			curPart[curPartIndex] = arr[secondIdx];
			curPartIndex++;
			secondIdx++;
		}
	}

	while (secondIdx <= r) {
		curPart[curPartIndex] = arr[secondIdx];
		curPartIndex++;
		secondIdx++;
	}
	while (firstIdx <= midIdx) {
		curPart[curPartIndex] = arr[firstIdx];
		curPartIndex++;
		firstIdx++;
	}

	for (curPartIndex = l; curPartIndex <= r; ++curPartIndex) {
		arr[curPartIndex] = curPart[curPartIndex - l];
	}
	
	pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, curPart);
}

/*
 *
 * <сводка>
 *   Функция doMergeSortFloat
 * </сводка>
 *
 * <описание>
 *   Рекурсивная функция, выполняющая сортировку слиянием, с типом Float
 * </описание>
 *
 */
static void doMergeSortFloat(CEcoLab1* pCMe, float* arr, int l, int r) {
	int midIdx, firstIdx, secondIdx;
	int curPartIndex = 0;
	float* curPart = 0;
	float swapValue;

	if (l >= r) {
		return;
	} else if (l + 1 == r) {
		if (arr[l] > arr[r]) {
			swapValue = arr[l];
			arr[l] = arr[r];
			arr[r] = swapValue;
		}
		return;
	}

	midIdx = (l + r) / 2;
	doMergeSortFloat(pCMe, arr, l, midIdx);
	doMergeSortFloat(pCMe, arr, midIdx + 1, r);

	firstIdx = l;
	secondIdx = midIdx + 1;
	curPartIndex = 0;
	curPart = (float*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(float) * (r - l + 1));

	while (firstIdx <= midIdx && secondIdx <= r) {
		if (arr[firstIdx] < arr[secondIdx]) {
			curPart[curPartIndex] = arr[firstIdx];
			curPartIndex++;
			firstIdx++;
		} else {
			curPart[curPartIndex] = arr[secondIdx];
			curPartIndex++;
			secondIdx++;
		}
	}

	while (secondIdx <= r) {
		curPart[curPartIndex] = arr[secondIdx];
		curPartIndex++;
		secondIdx++;
	}
	while (firstIdx <= midIdx) {
		curPart[curPartIndex] = arr[firstIdx];
		curPartIndex++;
		firstIdx++;
	}

	for (curPartIndex = l; curPartIndex <= r; ++curPartIndex) {
		arr[curPartIndex] = curPart[curPartIndex - l];
	}
	
	pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, curPart);
}

/*
 *
 * <сводка>
 *   Функция doMergeSortDouble
 * </сводка>
 *
 * <описание>
 *   Рекурсивная функция, выполняющая сортировку слиянием, с типом Double
 * </описание>
 *
 */
static void doMergeSortDouble(CEcoLab1* pCMe, double* arr, int l, int r) {
	int midIdx, firstIdx, secondIdx;
	int curPartIndex = 0;
	double* curPart = 0;
	double swapValue;

	if (l >= r) {
		return;
	} else if (l + 1 == r) {
		if (arr[l] > arr[r]) {
			swapValue = arr[l];
			arr[l] = arr[r];
			arr[r] = swapValue;
		}
		return;
	}

	midIdx = (l + r) / 2;
	doMergeSortDouble(pCMe, arr, l, midIdx);
	doMergeSortDouble(pCMe, arr, midIdx + 1, r);

	firstIdx = l;
	secondIdx = midIdx + 1;
	curPartIndex = 0;
	curPart = (double*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(double) * (r - l + 1));

	while (firstIdx <= midIdx && secondIdx <= r) {
		if (arr[firstIdx] < arr[secondIdx]) {
			curPart[curPartIndex] = arr[firstIdx];
			curPartIndex++;
			firstIdx++;
		} else {
			curPart[curPartIndex] = arr[secondIdx];
			curPartIndex++;
			secondIdx++;
		}
	}

	while (secondIdx <= r) {
		curPart[curPartIndex] = arr[secondIdx];
		curPartIndex++;
		secondIdx++;
	}
	while (firstIdx <= midIdx) {
		curPart[curPartIndex] = arr[firstIdx];
		curPartIndex++;
		firstIdx++;
	}

	for (curPartIndex = l; curPartIndex <= r; ++curPartIndex) {
		arr[curPartIndex] = curPart[curPartIndex - l];
	}
	
	pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, curPart);
}

/*
 *
 * <сводка>
 *   Функция doMergeSortLongDouble
 * </сводка>
 *
 * <описание>
 *   Рекурсивная функция, выполняющая сортировку слиянием, с типом LongDouble
 * </описание>
 *
 */
static void doMergeSortLongDouble(CEcoLab1* pCMe, long double* arr, int l, int r) {
	int midIdx, firstIdx, secondIdx;
	int curPartIndex = 0;
	long double* curPart = 0;
	long double swapValue;

	if (l >= r) {
		return;
	} else if (l + 1 == r) {
		if (arr[l] > arr[r]) {
			swapValue = arr[l];
			arr[l] = arr[r];
			arr[r] = swapValue;
		}
		return;
	}

	midIdx = (l + r) / 2;
	doMergeSortLongDouble(pCMe, arr, l, midIdx);
	doMergeSortLongDouble(pCMe, arr, midIdx + 1, r);

	firstIdx = l;
	secondIdx = midIdx + 1;
	curPartIndex = 0;
	curPart = (long double*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(long double) * (r - l + 1));

	while (firstIdx <= midIdx && secondIdx <= r) {
		if (arr[firstIdx] < arr[secondIdx]) {
			curPart[curPartIndex] = arr[firstIdx];
			curPartIndex++;
			firstIdx++;
		} else {
			curPart[curPartIndex] = arr[secondIdx];
			curPartIndex++;
			secondIdx++;
		}
	}

	while (secondIdx <= r) {
		curPart[curPartIndex] = arr[secondIdx];
		curPartIndex++;
		secondIdx++;
	}
	while (firstIdx <= midIdx) {
		curPart[curPartIndex] = arr[firstIdx];
		curPartIndex++;
		firstIdx++;
	}

	for (curPartIndex = l; curPartIndex <= r; ++curPartIndex) {
		arr[curPartIndex] = curPart[curPartIndex - l];
	}
	
	pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, curPart);
}

/*
 *
 * <сводка>
 *   Функция MergeSortInt
 * </сводка>
 *
 * <описание>
 *   Сортировка слиянием для типа Int
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_MergeSortInt(
	/* in */ IEcoLab1Ptr_t me,
	/* in */ int* arrayPtr,
	/* in */ int size
) {
	CEcoLab1* pCMe = (CEcoLab1*)me;

    if (me == 0 || arrayPtr == 0 || size <= 0) {
        return ERR_ECO_POINTER;
    }
    doMergeSortInt(pCMe, arrayPtr, 0, size - 1);
	return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция MergeSortLong
 * </сводка>
 *
 * <описание>
 *   Сортировка слиянием для типа Long
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_MergeSortLong(
	/* in */ IEcoLab1Ptr_t me,
	/* in */ long* arrayPtr,
	/* in */ int size
) {
	CEcoLab1* pCMe = (CEcoLab1*)me;

    if (me == 0 || arrayPtr == 0 || size <= 0) {
        return ERR_ECO_POINTER;
    }
    doMergeSortLong(pCMe, arrayPtr, 0, size - 1);
	return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция MergeSortFloat
 * </сводка>
 *
 * <описание>
 *   Сортировка слиянием для типа Float
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_MergeSortFloat(
	/* in */ IEcoLab1Ptr_t me,
	/* in */ float* arrayPtr,
	/* in */ int size
) {
	CEcoLab1* pCMe = (CEcoLab1*)me;

    if (me == 0 || arrayPtr == 0 || size <= 0) {
        return ERR_ECO_POINTER;
    }
    doMergeSortFloat(pCMe, arrayPtr, 0, size - 1);
	return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция MergeSortDouble
 * </сводка>
 *
 * <описание>
 *   Сортировка слиянием для типа Double
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_MergeSortDouble(
	/* in */ IEcoLab1Ptr_t me,
	/* in */ double* arrayPtr,
	/* in */ int size
) {
	CEcoLab1* pCMe = (CEcoLab1*)me;

    if (me == 0 || arrayPtr == 0 || size <= 0) {
        return ERR_ECO_POINTER;
    }
    doMergeSortDouble(pCMe, arrayPtr, 0, size - 1);
	return ERR_ECO_SUCCESES;
}

/*
 *
 * <сводка>
 *   Функция MergeSortLongDouble
 * </сводка>
 *
 * <описание>
 *   Сортировка слиянием для типа LongDouble
 * </описание>
 *
 */
static int16_t ECOCALLMETHOD CEcoLab1_MergeSortLongDouble(
	/* in */ IEcoLab1Ptr_t me,
	/* in */ long double* arrayPtr,
	/* in */ int size
) {
	CEcoLab1* pCMe = (CEcoLab1*)me;

    if (me == 0 || arrayPtr == 0 || size <= 0) {
        return ERR_ECO_POINTER;
    }
    doMergeSortLongDouble(pCMe, arrayPtr, 0, size - 1);
	return ERR_ECO_SUCCESES;
}


/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1_QueryInterface,
    CEcoLab1_AddRef,
    CEcoLab1_Release,
	CEcoLab1_MergeSortInt,
	CEcoLab1_MergeSortLong,
	CEcoLab1_MergeSortFloat,
	CEcoLab1_MergeSortDouble,
	CEcoLab1_MergeSortLongDouble,
};

/* Create Virtual Table IEcoCalculatorX */
IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTbl = {
    CEcoLab1_QueryInterfaceX,
    CEcoLab1_AddRefX,
    CEcoLab1_ReleaseX,
    CEcoLab1_AdditionX,
    CEcoLab1_SubtractionX,
};

/* Create Virtual Table IEcoCalculatorY */
IEcoCalculatorYVTbl g_xBD6414C29096423EA90C04D77AFD1CADVTbl = {
    CEcoLab1_QueryInterfaceY,
    CEcoLab1_AddRefY,
    CEcoLab1_ReleaseY,
    CEcoLab1_MultiplicationY,
    CEcoLab1_DivisionY,
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void **)&pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    /* Создание таблицы функций интерфейса IEcoCalculatorX */
    pCMe->m_pVTblIEcoCalculatorX = &g_x9322111622484742AE0682819447843DVTbl;
    /* Создание таблицы функций интерфейса IEcoCalculatorY */
    pCMe->m_pVTblIEcoCalculatorY = &g_xBD6414C29096423EA90C04D77AFD1CADVTbl;

    /* Инициализация данных */
	pCMe->m_pIEcoCalculatorX = 0;
	pCMe->m_pIEcoCalculatorY = 0;
	pCMe->m_pInnerUnknown = 0;
    pCMe->m_Name = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
