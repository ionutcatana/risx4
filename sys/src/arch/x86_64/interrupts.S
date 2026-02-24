        .intel_syntax noprefix

        .global intenable
        .global intdisable
        .global istub
        .global _vectors
        .global _idt                    # intdesc_t _idt[256]
        .global loadidt                 # loadidt(intdesc_t* desc)

        .extern idispatch

        .section .data
        .align  16
_vectors:                               # uint64_t _vectors[256]
        .quad   _int0
        .quad   _int1
        .quad   _int2
        .quad   _int3
        .quad   _int4
        .quad   _int5
        .quad   _int6
        .quad   _int7
        .quad   _int8
        .quad   _int9
        .quad   _int10
        .quad   _int11
        .quad   _int12
        .quad   _int13
        .quad   _int14
        .quad   _int15
        .quad   _int16
        .quad   _int17
        .quad   _int18
        .quad   _int19
        .quad   _int20
        .quad   _int21
        .quad   _int22
        .quad   _int23
        .quad   _int24
        .quad   _int25
        .quad   _int26
        .quad   _int27
        .quad   _int28
        .quad   _int29
        .quad   _int30
        .quad   _int31
        .quad   _int32
        .quad   _int33
        .quad   _int34
        .quad   _int35
        .quad   _int36
        .quad   _int37
        .quad   _int38
        .quad   _int39
        .quad   _int40
        .quad   _int41
        .quad   _int42
        .quad   _int43
        .quad   _int44
        .quad   _int45
        .quad   _int46
        .quad   _int47
        .quad   _int48
        .quad   _int49
        .quad   _int50
        .quad   _int51
        .quad   _int52
        .quad   _int53
        .quad   _int54
        .quad   _int55
        .quad   _int56
        .quad   _int57
        .quad   _int58
        .quad   _int59
        .quad   _int60
        .quad   _int61
        .quad   _int62
        .quad   _int63
        .quad   _int64
        .quad   _int65
        .quad   _int66
        .quad   _int67
        .quad   _int68
        .quad   _int69
        .quad   _int70
        .quad   _int71
        .quad   _int72
        .quad   _int73
        .quad   _int74
        .quad   _int75
        .quad   _int76
        .quad   _int77
        .quad   _int78
        .quad   _int79
        .quad   _int80
        .quad   _int81
        .quad   _int82
        .quad   _int83
        .quad   _int84
        .quad   _int85
        .quad   _int86
        .quad   _int87
        .quad   _int88
        .quad   _int89
        .quad   _int90
        .quad   _int91
        .quad   _int92
        .quad   _int93
        .quad   _int94
        .quad   _int95
        .quad   _int96
        .quad   _int97
        .quad   _int98
        .quad   _int99
        .quad   _int100
        .quad   _int101
        .quad   _int102
        .quad   _int103
        .quad   _int104
        .quad   _int105
        .quad   _int106
        .quad   _int107
        .quad   _int108
        .quad   _int109
        .quad   _int110
        .quad   _int111
        .quad   _int112
        .quad   _int113
        .quad   _int114
        .quad   _int115
        .quad   _int116
        .quad   _int117
        .quad   _int118
        .quad   _int119
        .quad   _int120
        .quad   _int121
        .quad   _int122
        .quad   _int123
        .quad   _int124
        .quad   _int125
        .quad   _int126
        .quad   _int127
        .quad   _int128
        .quad   _int129
        .quad   _int130
        .quad   _int131
        .quad   _int132
        .quad   _int133
        .quad   _int134
        .quad   _int135
        .quad   _int136
        .quad   _int137
        .quad   _int138
        .quad   _int139
        .quad   _int140
        .quad   _int141
        .quad   _int142
        .quad   _int143
        .quad   _int144
        .quad   _int145
        .quad   _int146
        .quad   _int147
        .quad   _int148
        .quad   _int149
        .quad   _int150
        .quad   _int151
        .quad   _int152
        .quad   _int153
        .quad   _int154
        .quad   _int155
        .quad   _int156
        .quad   _int157
        .quad   _int158
        .quad   _int159
        .quad   _int160
        .quad   _int161
        .quad   _int162
        .quad   _int163
        .quad   _int164
        .quad   _int165
        .quad   _int166
        .quad   _int167
        .quad   _int168
        .quad   _int169
        .quad   _int170
        .quad   _int171
        .quad   _int172
        .quad   _int173
        .quad   _int174
        .quad   _int175
        .quad   _int176
        .quad   _int177
        .quad   _int178
        .quad   _int179
        .quad   _int180
        .quad   _int181
        .quad   _int182
        .quad   _int183
        .quad   _int184
        .quad   _int185
        .quad   _int186
        .quad   _int187
        .quad   _int188
        .quad   _int189
        .quad   _int190
        .quad   _int191
        .quad   _int192
        .quad   _int193
        .quad   _int194
        .quad   _int195
        .quad   _int196
        .quad   _int197
        .quad   _int198
        .quad   _int199
        .quad   _int200
        .quad   _int201
        .quad   _int202
        .quad   _int203
        .quad   _int204
        .quad   _int205
        .quad   _int206
        .quad   _int207
        .quad   _int208
        .quad   _int209
        .quad   _int210
        .quad   _int211
        .quad   _int212
        .quad   _int213
        .quad   _int214
        .quad   _int215
        .quad   _int216
        .quad   _int217
        .quad   _int218
        .quad   _int219
        .quad   _int220
        .quad   _int221
        .quad   _int222
        .quad   _int223
        .quad   _int224
        .quad   _int225
        .quad   _int226
        .quad   _int227
        .quad   _int228
        .quad   _int229
        .quad   _int230
        .quad   _int231
        .quad   _int232
        .quad   _int233
        .quad   _int234
        .quad   _int235
        .quad   _int236
        .quad   _int237
        .quad   _int238
        .quad   _int239
        .quad   _int240
        .quad   _int241
        .quad   _int242
        .quad   _int243
        .quad   _int244
        .quad   _int245
        .quad   _int246
        .quad   _int247
        .quad   _int248
        .quad   _int249
        .quad   _int250
        .quad   _int251
        .quad   _int252
        .quad   _int253
        .quad   _int254
        .quad   _int255

        .section .bss
        .align  16
_idt:   .space  4096

        .code64
        .section .text
_int0:
        push    0
        push    0
        jmp     istub
_int1:
        push    0
        push    1
        jmp     istub
_int2:
        push    0
        push    2
        jmp     istub
_int3:
        push    0
        push    3
        jmp     istub
_int4:
        push    0
        push    4
        jmp     istub
_int5:
        push    0
        push    5
        jmp     istub
_int6:
        push    0
        push    6
        jmp     istub
_int7:
        push    0
        push    7
        jmp     istub
_int8:
        push    8
        jmp     istub
_int9:
        push    0
        push    9
        jmp     istub
_int10:
        push    10
        jmp     istub
_int11:
        push    11
        jmp     istub
_int12:
        push    12
        jmp     istub
_int13:
        push    13
        jmp     istub
_int14:
        push    14
        jmp     istub
_int15:
        push    0
        push    15
        jmp     istub
_int16:
        push    0
        push    16
        jmp     istub
_int17:
        push    17
        jmp     istub
_int18:
        push    0
        push    18
        jmp     istub
_int19:
        push    0
        push    19
        jmp     istub
_int20:
        push    0
        push    20
        jmp     istub
_int21:
        push    21
        jmp     istub
_int22:
        push    0
        push    22
        jmp     istub
_int23:
        push    0
        push    23
        jmp     istub
_int24:
        push    0
        push    24
        jmp     istub
_int25:
        push    0
        push    25
        jmp     istub
_int26:
        push    0
        push    26
        jmp     istub
_int27:
        push    0
        push    27
        jmp     istub
_int28:
        push    0
        push    28
        jmp     istub
_int29:
        push    29
        jmp     istub
_int30:
        push    30
        jmp     istub
_int31:
        push    0
        push    31
        jmp     istub
_int32:
        push    0
        push    32
        jmp     istub
_int33:
        push    0
        push    33
        jmp     istub
_int34:
        push    0
        push    34
        jmp     istub
_int35:
        push    0
        push    35
        jmp     istub
_int36:
        push    0
        push    36
        jmp     istub
_int37:
        push    0
        push    37
        jmp     istub
_int38:
        push    0
        push    38
        jmp     istub
_int39:
        push    0
        push    39
        jmp     istub
_int40:
        push    0
        push    40
        jmp     istub
_int41:
        push    0
        push    41
        jmp     istub
_int42:
        push    0
        push    42
        jmp     istub
_int43:
        push    0
        push    43
        jmp     istub
_int44:
        push    0
        push    44
        jmp     istub
_int45:
        push    0
        push    45
        jmp     istub
_int46:
        push    0
        push    46
        jmp     istub
_int47:
        push    0
        push    47
        jmp     istub
_int48:
        push    0
        push    48
        jmp     istub
_int49:
        push    0
        push    49
        jmp     istub
_int50:
        push    0
        push    50
        jmp     istub
_int51:
        push    0
        push    51
        jmp     istub
_int52:
        push    0
        push    52
        jmp     istub
_int53:
        push    0
        push    53
        jmp     istub
_int54:
        push    0
        push    54
        jmp     istub
_int55:
        push    0
        push    55
        jmp     istub
_int56:
        push    0
        push    56
        jmp     istub
_int57:
        push    0
        push    57
        jmp     istub
_int58:
        push    0
        push    58
        jmp     istub
_int59:
        push    0
        push    59
        jmp     istub
_int60:
        push    0
        push    60
        jmp     istub
_int61:
        push    0
        push    61
        jmp     istub
_int62:
        push    0
        push    62
        jmp     istub
_int63:
        push    0
        push    63
        jmp     istub
_int64:
        push    0
        push    64
        jmp     istub
_int65:
        push    0
        push    65
        jmp     istub
_int66:
        push    0
        push    66
        jmp     istub
_int67:
        push    0
        push    67
        jmp     istub
_int68:
        push    0
        push    68
        jmp     istub
_int69:
        push    0
        push    69
        jmp     istub
_int70:
        push    0
        push    70
        jmp     istub
_int71:
        push    0
        push    71
        jmp     istub
_int72:
        push    0
        push    72
        jmp     istub
_int73:
        push    0
        push    73
        jmp     istub
_int74:
        push    0
        push    74
        jmp     istub
_int75:
        push    0
        push    75
        jmp     istub
_int76:
        push    0
        push    76
        jmp     istub
_int77:
        push    0
        push    77
        jmp     istub
_int78:
        push    0
        push    78
        jmp     istub
_int79:
        push    0
        push    79
        jmp     istub
_int80:
        push    0
        push    80
        jmp     istub
_int81:
        push    0
        push    81
        jmp     istub
_int82:
        push    0
        push    82
        jmp     istub
_int83:
        push    0
        push    83
        jmp     istub
_int84:
        push    0
        push    84
        jmp     istub
_int85:
        push    0
        push    85
        jmp     istub
_int86:
        push    0
        push    86
        jmp     istub
_int87:
        push    0
        push    87
        jmp     istub
_int88:
        push    0
        push    88
        jmp     istub
_int89:
        push    0
        push    89
        jmp     istub
_int90:
        push    0
        push    90
        jmp     istub
_int91:
        push    0
        push    91
        jmp     istub
_int92:
        push    0
        push    92
        jmp     istub
_int93:
        push    0
        push    93
        jmp     istub
_int94:
        push    0
        push    94
        jmp     istub
_int95:
        push    0
        push    95
        jmp     istub
_int96:
        push    0
        push    96
        jmp     istub
_int97:
        push    0
        push    97
        jmp     istub
_int98:
        push    0
        push    98
        jmp     istub
_int99:
        push    0
        push    99
        jmp     istub
_int100:
        push    0
        push    100
        jmp     istub
_int101:
        push    0
        push    101
        jmp     istub
_int102:
        push    0
        push    102
        jmp     istub
_int103:
        push    0
        push    103
        jmp     istub
_int104:
        push    0
        push    104
        jmp     istub
_int105:
        push    0
        push    105
        jmp     istub
_int106:
        push    0
        push    106
        jmp     istub
_int107:
        push    0
        push    107
        jmp     istub
_int108:
        push    0
        push    108
        jmp     istub
_int109:
        push    0
        push    109
        jmp     istub
_int110:
        push    0
        push    110
        jmp     istub
_int111:
        push    0
        push    111
        jmp     istub
_int112:
        push    0
        push    112
        jmp     istub
_int113:
        push    0
        push    113
        jmp     istub
_int114:
        push    0
        push    114
        jmp     istub
_int115:
        push    0
        push    115
        jmp     istub
_int116:
        push    0
        push    116
        jmp     istub
_int117:
        push    0
        push    117
        jmp     istub
_int118:
        push    0
        push    118
        jmp     istub
_int119:
        push    0
        push    119
        jmp     istub
_int120:
        push    0
        push    120
        jmp     istub
_int121:
        push    0
        push    121
        jmp     istub
_int122:
        push    0
        push    122
        jmp     istub
_int123:
        push    0
        push    123
        jmp     istub
_int124:
        push    0
        push    124
        jmp     istub
_int125:
        push    0
        push    125
        jmp     istub
_int126:
        push    0
        push    126
        jmp     istub
_int127:
        push    0
        push    127
        jmp     istub
_int128:
        push    0
        push    128
        jmp     istub
_int129:
        push    0
        push    129
        jmp     istub
_int130:
        push    0
        push    130
        jmp     istub
_int131:
        push    0
        push    131
        jmp     istub
_int132:
        push    0
        push    132
        jmp     istub
_int133:
        push    0
        push    133
        jmp     istub
_int134:
        push    0
        push    134
        jmp     istub
_int135:
        push    0
        push    135
        jmp     istub
_int136:
        push    0
        push    136
        jmp     istub
_int137:
        push    0
        push    137
        jmp     istub
_int138:
        push    0
        push    138
        jmp     istub
_int139:
        push    0
        push    139
        jmp     istub
_int140:
        push    0
        push    140
        jmp     istub
_int141:
        push    0
        push    141
        jmp     istub
_int142:
        push    0
        push    142
        jmp     istub
_int143:
        push    0
        push    143
        jmp     istub
_int144:
        push    0
        push    144
        jmp     istub
_int145:
        push    0
        push    145
        jmp     istub
_int146:
        push    0
        push    146
        jmp     istub
_int147:
        push    0
        push    147
        jmp     istub
_int148:
        push    0
        push    148
        jmp     istub
_int149:
        push    0
        push    149
        jmp     istub
_int150:
        push    0
        push    150
        jmp     istub
_int151:
        push    0
        push    151
        jmp     istub
_int152:
        push    0
        push    152
        jmp     istub
_int153:
        push    0
        push    153
        jmp     istub
_int154:
        push    0
        push    154
        jmp     istub
_int155:
        push    0
        push    155
        jmp     istub
_int156:
        push    0
        push    156
        jmp     istub
_int157:
        push    0
        push    157
        jmp     istub
_int158:
        push    0
        push    158
        jmp     istub
_int159:
        push    0
        push    159
        jmp     istub
_int160:
        push    0
        push    160
        jmp     istub
_int161:
        push    0
        push    161
        jmp     istub
_int162:
        push    0
        push    162
        jmp     istub
_int163:
        push    0
        push    163
        jmp     istub
_int164:
        push    0
        push    164
        jmp     istub
_int165:
        push    0
        push    165
        jmp     istub
_int166:
        push    0
        push    166
        jmp     istub
_int167:
        push    0
        push    167
        jmp     istub
_int168:
        push    0
        push    168
        jmp     istub
_int169:
        push    0
        push    169
        jmp     istub
_int170:
        push    0
        push    170
        jmp     istub
_int171:
        push    0
        push    171
        jmp     istub
_int172:
        push    0
        push    172
        jmp     istub
_int173:
        push    0
        push    173
        jmp     istub
_int174:
        push    0
        push    174
        jmp     istub
_int175:
        push    0
        push    175
        jmp     istub
_int176:
        push    0
        push    176
        jmp     istub
_int177:
        push    0
        push    177
        jmp     istub
_int178:
        push    0
        push    178
        jmp     istub
_int179:
        push    0
        push    179
        jmp     istub
_int180:
        push    0
        push    180
        jmp     istub
_int181:
        push    0
        push    181
        jmp     istub
_int182:
        push    0
        push    182
        jmp     istub
_int183:
        push    0
        push    183
        jmp     istub
_int184:
        push    0
        push    184
        jmp     istub
_int185:
        push    0
        push    185
        jmp     istub
_int186:
        push    0
        push    186
        jmp     istub
_int187:
        push    0
        push    187
        jmp     istub
_int188:
        push    0
        push    188
        jmp     istub
_int189:
        push    0
        push    189
        jmp     istub
_int190:
        push    0
        push    190
        jmp     istub
_int191:
        push    0
        push    191
        jmp     istub
_int192:
        push    0
        push    192
        jmp     istub
_int193:
        push    0
        push    193
        jmp     istub
_int194:
        push    0
        push    194
        jmp     istub
_int195:
        push    0
        push    195
        jmp     istub
_int196:
        push    0
        push    196
        jmp     istub
_int197:
        push    0
        push    197
        jmp     istub
_int198:
        push    0
        push    198
        jmp     istub
_int199:
        push    0
        push    199
        jmp     istub
_int200:
        push    0
        push    200
        jmp     istub
_int201:
        push    0
        push    201
        jmp     istub
_int202:
        push    0
        push    202
        jmp     istub
_int203:
        push    0
        push    203
        jmp     istub
_int204:
        push    0
        push    204
        jmp     istub
_int205:
        push    0
        push    205
        jmp     istub
_int206:
        push    0
        push    206
        jmp     istub
_int207:
        push    0
        push    207
        jmp     istub
_int208:
        push    0
        push    208
        jmp     istub
_int209:
        push    0
        push    209
        jmp     istub
_int210:
        push    0
        push    210
        jmp     istub
_int211:
        push    0
        push    211
        jmp     istub
_int212:
        push    0
        push    212
        jmp     istub
_int213:
        push    0
        push    213
        jmp     istub
_int214:
        push    0
        push    214
        jmp     istub
_int215:
        push    0
        push    215
        jmp     istub
_int216:
        push    0
        push    216
        jmp     istub
_int217:
        push    0
        push    217
        jmp     istub
_int218:
        push    0
        push    218
        jmp     istub
_int219:
        push    0
        push    219
        jmp     istub
_int220:
        push    0
        push    220
        jmp     istub
_int221:
        push    0
        push    221
        jmp     istub
_int222:
        push    0
        push    222
        jmp     istub
_int223:
        push    0
        push    223
        jmp     istub
_int224:
        push    0
        push    224
        jmp     istub
_int225:
        push    0
        push    225
        jmp     istub
_int226:
        push    0
        push    226
        jmp     istub
_int227:
        push    0
        push    227
        jmp     istub
_int228:
        push    0
        push    228
        jmp     istub
_int229:
        push    0
        push    229
        jmp     istub
_int230:
        push    0
        push    230
        jmp     istub
_int231:
        push    0
        push    231
        jmp     istub
_int232:
        push    0
        push    232
        jmp     istub
_int233:
        push    0
        push    233
        jmp     istub
_int234:
        push    0
        push    234
        jmp     istub
_int235:
        push    0
        push    235
        jmp     istub
_int236:
        push    0
        push    236
        jmp     istub
_int237:
        push    0
        push    237
        jmp     istub
_int238:
        push    0
        push    238
        jmp     istub
_int239:
        push    0
        push    239
        jmp     istub
_int240:
        push    0
        push    240
        jmp     istub
_int241:
        push    0
        push    241
        jmp     istub
_int242:
        push    0
        push    242
        jmp     istub
_int243:
        push    0
        push    243
        jmp     istub
_int244:
        push    0
        push    244
        jmp     istub
_int245:
        push    0
        push    245
        jmp     istub
_int246:
        push    0
        push    246
        jmp     istub
_int247:
        push    0
        push    247
        jmp     istub
_int248:
        push    0
        push    248
        jmp     istub
_int249:
        push    0
        push    249
        jmp     istub
_int250:
        push    0
        push    250
        jmp     istub
_int251:
        push    0
        push    251
        jmp     istub
_int252:
        push    0
        push    252
        jmp     istub
_int253:
        push    0
        push    253
        jmp     istub
_int254:
        push    0
        push    254
        jmp     istub
_int255:
        push    0
        push    255
        jmp     istub

#       push    ss                      # happens automatically
#       push    rsp
#       push    rflags
#       push    cs
#       push    rip
istub:  push    rax                     # save the general purpose registers
        push    rbx
        push    rcx
        push    rdx
        push    rsi
        push    rdi
#       push    rsp
        push    rbp
        push    r8
        push    r9
        push    r10
        push    r11
        push    r12
        push    r13
        push    r14
        push    r15

        mov     rdi, rsp                # move trapframe into first argument
        call    idispatch               # void idispatch(trapframe_t* tf)

        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     r11
        pop     r10
        pop     r9
        pop     r8
        pop     rbp
#       pop     rsp
        pop     rdi
        pop     rsi
        pop     rdx
        pop     rcx
        pop     rbx
        pop     rax

        add     rsp, 16                 # remove the vector number & error code

        iretq
#       pop     rip                     # happens automatically
#       pop     cs
#       pop     rflags
#       pop     rsp
#       pop     ss

intenable:
        sti
        ret

intdisable:
        cli
        ret

        .code64
        .section .text
loadidt:
        lidt    [rdi]
        ret
