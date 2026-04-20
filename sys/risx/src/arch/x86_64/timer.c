#include "commonarch/timer.h"
#include "arch/x86_64/specific/lapic.h"
#include "arch/x86_64/interrupts.h"
#include "commonarch/io.h"
#include "lib/printf.h"
#include <stdint.h>

// PIT (i8254) constants, used only for one-shot LAPIC timer calibration.
// The PIT oscillator runs at ~1.193182 MHz.
#define PIT_FREQ        1193182
#define PIT_CHANNEL2    0x42
#define PIT_CMD         0x43
#define PIT_PORT_B      0x61

// calibrate over ~10 ms (11932 ticks at 1.193182 MHz)
#define PIT_CAL_TICKS   11932
#define PIT_CAL_MS      10

static volatile uint64_t ticks;

uint64_t
tickcount(void)
{
    return ticks;
}

void
timertick(void)
{
    ticks++;
}

// use pit channel 2 in one-shot mode to measure how many lapic timer ticks elapse in pit_cal_ms milliseconds.
// returns lapic ticks per ms.
static uint32_t
calibrate_lapic_timer(void)
{
    uint32_t lapic_start, lapic_end;
    uint8_t portb;

    // configure PIT channel 2 for one-shot mode, lo/hi byte
    writes(PIT_CMD, 0xB0); // channel 2, lobyte/hibyte, mode 0

    // gate off channel 2
    portb = reads(PIT_PORT_B);
    writes(PIT_PORT_B, (portb & 0xFC) | 0x01); // gate high, speaker off

    // load the count
    writes(PIT_CHANNEL2, (uint8_t)(PIT_CAL_TICKS & 0xFF));
    writes(PIT_CHANNEL2, (uint8_t)((PIT_CAL_TICKS >> 8) & 0xFF));

    // start LAPIC timer in one-shot mode, divider 1
    lapicwrite(LAPIC_TDCR, LAPIC_TDCR_DIV1);
    lapicwrite(LAPIC_TIMER, LAPIC_LVT_MASKED | LAPIC_TIMER_ONESHOT);
    lapicwrite(LAPIC_TICR, 0xFFFFFFFF);

    // wait for PIT channel 2 output to go high
    while ((reads(PIT_PORT_B) & 0x20) == 0)
        ;

    // read LAPIC timer current count
    lapic_end = lapicread(LAPIC_TCCR);
    lapic_start = 0xFFFFFFFF;

    // mask the timer again
    lapicwrite(LAPIC_TIMER, LAPIC_LVT_MASKED);

    return ((lapic_start - lapic_end) / PIT_CAL_MS);
}

// initialize the lapic timer on the current cpu in periodic mode at timer_hz ticks per second.
// must be called after initlapic().
void
inittimer(void)
{
    uint32_t ticks_per_ms;
    uint32_t initial_count;

    ticks_per_ms = calibrate_lapic_timer();
    initial_count = ticks_per_ms * (1000 / TIMER_HZ);

    printf("TIMER: LAPIC ticks/ms=%u, initial_count=%u (%u Hz)\n",
        ticks_per_ms,
        initial_count,
        TIMER_HZ);

    // program lapic timer: periodic, divider 1
    lapicwrite(LAPIC_TDCR, LAPIC_TDCR_DIV1);
    lapicwrite(LAPIC_TIMER, VEC_LAPIC_TIMER | LAPIC_TIMER_PERIODIC);
    lapicwrite(LAPIC_TICR, initial_count);
}
