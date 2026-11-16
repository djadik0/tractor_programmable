# Подключение тракторных команд к PWM

В main.c настроен один таймер TIM1 с тремя каналами: CH1 — ход, CH2 — стрела, CH3 — ковш. Работающий канал имеет сравнение 700 при периоде 999, остановленный — 0. Каналы запускаются при инициализации, команда меняет заполнение. Исходный tractor_top передаёт направление, поэтому мощность фиксирована.

Цепочка: Tractor_OnUartCommand → Tractor_Process → callbacks tractor_pwm.c → TIM1. Например, TRACTOR_CMD_DRIVE_FORWARD включает CH1, TRACTOR_CMD_DRIVE_STOP выключает его. UART-декодер, вызывающий Tractor_OnUartCommand, в предоставленных файлах отсутствует. Исходный tractor_top хранит одну ожидающую команду.

Функции TractorBoard_SetDirection, TractorBoard_SetSteering и TractorBoard_SensorActive пока заглушки: разводка и тип драйвера неизвестны. Вперёд и назад создают одинаковый PWM; физическое направление, руление и концевики не подключены. Настройте выводы CH2 и CH3 в HAL_TIM_MspPostInit и проверьте поддержку каналов выбранным STM32. Не подключайте моторы до реализации и проверки драйвера и концевиков.

Добавьте tractor_top.c и tractor_pwm.c и заголовки в проект STM32CubeIDE. Полный проект, main.h, HAL и настройки платы не приложены. Пакетный UART, регулировка мощности командами, остановка при потере связи и телеметрия не реализованы.

## Проверки на компьютере

В каталоге `tests` находятся две проверки с имитацией HAL. `test_pwm.c` проверяет обработку команд, PWM и концевик; `test_main.c` запускает настоящий `main.c` с подменой аппаратных функций и проверяет настройку каналов, команды и общую остановку. Это не проверка физического сигнала, UART или работы платы.

Запуск из корня проекта в PowerShell при наличии GCC в PATH:

```powershell
New-Item -ItemType Directory -Force build-test | Out-Null
gcc -std=c11 -Wall -Wextra -Werror -I tests -I . tractor_top.c tractor_pwm.c tests/test_pwm.c -o build-test/test_pwm.exe
if ($LASTEXITCODE -ne 0) { throw 'Ошибка сборки test_pwm' }
./build-test/test_pwm.exe
if ($LASTEXITCODE -ne 0) { throw 'Ошибка test_pwm' }
gcc -std=c11 -Wall -Wextra -Werror -DTEST_FULL_MAIN -Dmain=firmware_main -DTractor_Process=Test_Process -I tests -I . -c main.c -o build-test/main.o
if ($LASTEXITCODE -ne 0) { throw 'Ошибка сборки main.c' }
gcc -std=c11 -Wall -Wextra -Werror -DTEST_FULL_MAIN -I tests -I . tractor_top.c tractor_pwm.c tests/test_main.c build-test/main.o -o build-test/test_main.exe
if ($LASTEXITCODE -ne 0) { throw 'Ошибка сборки test_main' }
./build-test/test_main.exe
if ($LASTEXITCODE -ne 0) { throw 'Ошибка test_main' }
```
