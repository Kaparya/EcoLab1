/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int __cdecl compareInt(const void *xPtr, const void *yPtr) {
    int x = *(int *)xPtr, y = *(int *)yPtr;
    return (x > y) - (x < y);
}
int __cdecl compareLong(const void *xPtr, const void *yPtr) {
    long x = *(long *)xPtr, y = *(long *)yPtr;
    return (x > y) - (x < y);
}

int randInt(int min, int max) {
    return rand() % (max - min) + min;
}

long randLong(long min, long max) {
    return rand() % (max - min) + min;
}

void testArrayInt(IEcoLab1* pIEcoLab1, IEcoMemoryAllocator1* pIMem, uint32_t curSize, FILE* file) {
	int* randomArray = 0;
	int* randomArraySTD;
	int16_t result = 0;

	uint32_t i = 0;

	clock_t begin, beginSTD;
    clock_t end, endSTD;
	double time, timeSTD;

	randomArray = (int*) pIMem->pVTbl->Alloc(pIMem, sizeof(int) * curSize);
	randomArraySTD = (int *) pIMem->pVTbl->Alloc(pIMem, sizeof(int) * curSize);
	// ---------------
	for (i = 0; i < curSize; ++i) {
		randomArray[i] = randInt(-1000,1000);
	}
	pIMem->pVTbl->Copy(pIMem, randomArraySTD, randomArray, sizeof(int) * curSize);

	beginSTD = clock();
	qsort(randomArraySTD, curSize, sizeof(int), compareInt); 
	endSTD = clock();
	timeSTD = (double)(endSTD - beginSTD);

	begin = clock();
	result = pIEcoLab1->pVTbl->MergeSortInt(pIEcoLab1, randomArray, curSize); 
	end = clock();
	time = (double)(end - begin);

	printf("Int: MergeSorted array of %d elements in %f:\n", curSize, time);
	printf("Int: STD sorted array of %d elements in %f:\n", curSize, timeSTD);

	for (i = 0; i < curSize; ++i) {
		if (randomArray[i] != randomArraySTD[i]) {
			printf("\nInt: MISMATCH: %d %d\n", randomArray[i], randomArraySTD[i]);
			fprintf(file, "Int: MISMATCH: %d %d\n", randomArray[i], randomArraySTD[i]);
			break;
		}
	}

	/* Освобождение блока памяти */
	pIMem->pVTbl->Free(pIMem, randomArray);
	pIMem->pVTbl->Free(pIMem, randomArraySTD);

    fprintf(file, "%s,%s,%d,%lf\n", "MergeSort", "Int", curSize, time);
    fprintf(file, "%s,%s,%d,%lf\n", "QuickSTD", "Int", curSize, timeSTD);
}

void testArrayLong(IEcoLab1* pIEcoLab1, IEcoMemoryAllocator1* pIMem, uint32_t curSize, FILE* file) {
	long* randomArray = 0;
	long* randomArraySTD;
	int16_t result = 0;

	uint32_t i = 0;

	clock_t begin, beginSTD;
    clock_t end, endSTD;
	double time, timeSTD;

	randomArray = (long*) pIMem->pVTbl->Alloc(pIMem, sizeof(long) * curSize);
	randomArraySTD = (long *) pIMem->pVTbl->Alloc(pIMem, sizeof(long) * curSize);
	// ---------------
	for (i = 0; i < curSize; ++i) {
		randomArray[i] = randLong(-100000,100000);
	}
	pIMem->pVTbl->Copy(pIMem, randomArraySTD, randomArray, sizeof(long) * curSize);

	beginSTD = clock();
	qsort(randomArraySTD, curSize, sizeof(long), compareLong); 
	endSTD = clock();
	timeSTD = (double)(endSTD - beginSTD);

	begin = clock();
	result = pIEcoLab1->pVTbl->MergeSortLong(pIEcoLab1, randomArray, curSize); 
	end = clock();
	time = (double)(end - begin);

	printf("Long: MergeSorted array of %d elements in %f:\n", curSize, time);
	printf("Long: STD sorted array of %d elements in %f:\n", curSize, timeSTD);

	for (i = 0; i < curSize; ++i) {
		if (randomArray[i] != randomArraySTD[i]) {
			printf("\nLong: MISMATCH: %d %d\n", randomArray[i], randomArraySTD[i]);
			fprintf(file, "Long: MISMATCH: %d %d\n", randomArray[i], randomArraySTD[i]);
			break;
		}
	}

	/* Освобождение блока памяти */
	pIMem->pVTbl->Free(pIMem, randomArray);
	pIMem->pVTbl->Free(pIMem, randomArraySTD);

    fprintf(file, "%s,%s,%d,%lf\n", "MergeSort", "Long", curSize, time);
    fprintf(file, "%s,%s,%d,%lf\n", "QuickSTD", "Long", curSize, timeSTD);
}

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;

	int sizes[10] = {10, 100, 1000, 10000, 100000, 1000000, 3000000, 7000000, 10000000};
	int sizesNumber = 10;

	uint32_t i = 0;

    FILE* file;

    srand(time(0));

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

	file = fopen("stats.csv", "w");
	fprintf(file, "SortingName,DataType,ArraySize,Time\n");


	for (i = 0; i < sizesNumber; ++i) {
		testArrayInt(pIEcoLab1, pIMem, sizes[i], file);
		testArrayLong(pIEcoLab1, pIMem, sizes[i], file);
	}

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }


    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}

