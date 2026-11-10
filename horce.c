#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HORSE_NUM 6
#define GOAL 50
#define START_MONEY 1000

const char* condition[HORSE_NUM] = {
    "최상", "좋음", "보통", "피곤함", "나쁨", "최악"
};
int rate[HORSE_NUM] = { 35, 25, 17, 11, 7, 5 };

int cond[HORSE_NUM];
int pos[HORSE_NUM];

void shuffle_condition(void)
{
    int i, j, tmp;

    for (i = 0; i < HORSE_NUM; i++)
        cond[i] = i;
    for (i = HORSE_NUM - 1; i > 0; i--) {
        j = rand() % (i + 1);
        tmp = cond[i];
        cond[i] = cond[j];
        cond[j] = tmp;
    }
}

void print_info(void)
{
    int i;

    printf("\n번호\t컨디션\t승률\t배당\n");
    for (i = 0; i < HORSE_NUM; i++)
        printf("%d번\t%s\t%d%%\t%.1f배\n", i + 1, condition[cond[i]],
            rate[cond[i]], 90.0 / rate[cond[i]]);
    printf("\n");
}

int pick_winner(void)
{
    int i, sum = 0, r = rand() % 100;

    for (i = 0; i < HORSE_NUM; i++) {
        sum += rate[cond[i]];
        if (r < sum)
            return i;
    }
    return HORSE_NUM - 1;
}

void print_track(void)
{
    int i, j;

    system("cls");
    for (i = 0; i < HORSE_NUM; i++) {
        printf("%d번 |", i + 1);
        for (j = 0; j < pos[i]; j++)
            printf("-");
        printf("%d", i + 1);
        for (j = pos[i]; j < GOAL; j++)
            printf(" ");
        printf("|\n");
    }
}

void race(int winner)
{
    int i, step, max, finish = 0;

    for (i = 0; i < HORSE_NUM; i++)
        pos[i] = 0;

    while (!finish) {
        max = 0;
        for (i = 0; i < HORSE_NUM; i++)
            if (i != winner && pos[i] > max)
                max = pos[i];

        for (i = 0; i < HORSE_NUM; i++) {
            step = rand() % 3 + 1;
            if (i == winner) {
                if (pos[i] < max)
                    step++;
            }
            else {
                if (pos[i] + step > pos[winner] + 5)
                    step = 0;
                if (pos[i] + step >= GOAL)
                    step = GOAL - 1 - pos[i];
            }
            pos[i] += step;
        }

        if (pos[winner] >= GOAL) {
            pos[winner] = GOAL;
            finish = 1;
        }
    }
    print_track();
}

int main(void)
{
    int money = START_MONEY;
    int num, bet, winner, prize, again;

    srand((unsigned)time(NULL));

    printf("===== 경마 게임 =====\n");
    printf("우승할 말을 맞히면 배당만큼 돈을 받습니다.\n");
    printf("시작 소지금 : %d원\n", money);

    while (money > 0) {
        shuffle_condition();
        print_info();

        do {
            printf("몇 번 말에 거시겠습니까? (1~%d, 종료 0) : ", HORSE_NUM);
            scanf("%d", &num);
        } while (num < 0 || num > HORSE_NUM);
        if (num == 0)
            break;

        do {
            printf("베팅 금액 (1~%d원) : ", money);
            scanf("%d", &bet);
        } while (bet < 1 || bet > money);
        money -= bet;

        winner = pick_winner();
        race(winner);

        printf("\n우승 : %d번 말\n", winner + 1);
        if (num - 1 == winner) {
            prize = bet * 90 / rate[cond[winner]];
            money += prize;
            printf("축하합니다! %d원을 받았습니다.\n", prize);
        }
        else {
            printf("아쉽네요. %d원을 잃었습니다.\n", bet);
        }
        printf("현재 소지금 : %d원\n", money);

        if (money <= 0) {
            printf("소지금이 0원이 되었습니다.\n");
            break;
        }

        printf("\n계속하시겠습니까? (1: 계속, 0: 종료) : ");
        scanf("%d", &again);
        if (again == 0)
            break;
    }

    printf("\n게임 종료! 최종 소지금 : %d원\n", money);
    return 0;
}