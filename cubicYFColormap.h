#ifndef CUBICYFCOLORMAP_H
#define CUBICYFCOLORMAP_H
#include <QColor>
const float    MAXFRAMELEN            = 2500;

class CubicYFColorMap{
private:
    float cubicYF[256][3] = {{0.5151,   0.0482,   0.6697},
                             {0.5159,   0.0561,   0.6785},
                             {0.5166,   0.0639,   0.6872},
                             {0.5172,   0.0717,   0.6959},
                             {0.5178,   0.0795,   0.7045},
                             {0.5182,   0.0872,   0.7130},
                             {0.5186,   0.0949,   0.7214},
                             {0.5189,   0.1025,   0.7297},
                             {0.5192,   0.1101,   0.7380},
                             {0.5194,   0.1176,   0.7461},
                             {0.5196,   0.1251,   0.7542},
                             {0.5197,   0.1326,   0.7622},
                             {0.5198,   0.1400,   0.7701},
                             {0.5198,   0.1473,   0.7780},
                             {0.5199,   0.1546,   0.7857},
                             {0.5199,   0.1618,   0.7933},
                             {0.5199,   0.1690,   0.8009},
                             {0.5199,   0.1762,   0.8083},
                             {0.5197,   0.1833,   0.8157},
                             {0.5192,   0.1904,   0.8232},
                             {0.5184,   0.1975,   0.8306},
                             {0.5172,   0.2046,   0.8381},
                             {0.5158,   0.2116,   0.8455},
                             {0.5142,   0.2186,   0.8528},
                             {0.5124,   0.2256,   0.8601},
                             {0.5103,   0.2325,   0.8673},
                             {0.5082,   0.2393,   0.8743},
                             {0.5059,   0.2461,   0.8812},
                             {0.5035,   0.2528,   0.8879},
                             {0.5010,   0.2594,   0.8945},
                             {0.4985,   0.2660,   0.9008},
                             {0.4959,   0.2724,   0.9068},
                             {0.4934,   0.2788,   0.9127},
                             {0.4909,   0.2850,   0.9182},
                             {0.4884,   0.2912,   0.9234},
                             {0.4858,   0.2972,   0.9286},
                             {0.4830,   0.3032,   0.9339},
                             {0.4799,   0.3090,   0.9394},
                             {0.4766,   0.3147,   0.9449},
                             {0.4731,   0.3203,   0.9503},
                             {0.4694,   0.3259,   0.9557},
                             {0.4657,   0.3314,   0.9610},
                             {0.4618,   0.3368,   0.9661},
                             {0.4580,   0.3423,   0.9709},
                             {0.4541,   0.3476,   0.9753},
                             {0.4502,   0.3530,   0.9794},
                             {0.4465,   0.3584,   0.9830},
                             {0.4428,   0.3637,   0.9861},
                             {0.4392,   0.3691,   0.9886},
                             {0.4358,   0.3745,   0.9905},
                             {0.4326,   0.3800,   0.9917},
                             {0.4297,   0.3855,   0.9921},
                             {0.4270,   0.3910,   0.9920},
                             {0.4243,   0.3966,   0.9918},
                             {0.4218,   0.4022,   0.9915},
                             {0.4194,   0.4078,   0.9911},
                             {0.4171,   0.4134,   0.9905},
                             {0.4149,   0.4189,   0.9899},
                             {0.4126,   0.4245,   0.9891},
                             {0.4105,   0.4301,   0.9883},
                             {0.4083,   0.4357,   0.9874},
                             {0.4061,   0.4412,   0.9863},
                             {0.4039,   0.4468,   0.9853},
                             {0.4017,   0.4523,   0.9841},
                             {0.3994,   0.4577,   0.9829},
                             {0.3970,   0.4632,   0.9816},
                             {0.3945,   0.4686,   0.9803},
                             {0.3920,   0.4739,   0.9789},
                             {0.3893,   0.4792,   0.9775},
                             {0.3865,   0.4845,   0.9758},
                             {0.3836,   0.4897,   0.9734},
                             {0.3805,   0.4949,   0.9706},
                             {0.3774,   0.5001,   0.9672},
                             {0.3742,   0.5053,   0.9635},
                             {0.3709,   0.5104,   0.9594},
                             {0.3676,   0.5155,   0.9549},
                             {0.3642,   0.5206,   0.9502},
                             {0.3608,   0.5257,   0.9453},
                             {0.3574,   0.5307,   0.9403},
                             {0.3539,   0.5357,   0.9352},
                             {0.3505,   0.5407,   0.9300},
                             {0.3471,   0.5456,   0.9249},
                             {0.3437,   0.5505,   0.9198},
                             {0.3403,   0.5554,   0.9149},
                             {0.3370,   0.5602,   0.9101},
                             {0.3337,   0.5650,   0.9056},
                             {0.3305,   0.5698,   0.9012},
                             {0.3273,   0.5745,   0.8969},
                             {0.3241,   0.5792,   0.8926},
                             {0.3209,   0.5838,   0.8883},
                             {0.3178,   0.5884,   0.8840},
                             {0.3146,   0.5930,   0.8797},
                             {0.3115,   0.5976,   0.8754},
                             {0.3083,   0.6021,   0.8711},
                             {0.3052,   0.6067,   0.8667},
                             {0.3020,   0.6111,   0.8622},
                             {0.2989,   0.6156,   0.8577},
                             {0.2957,   0.6200,   0.8531},
                             {0.2925,   0.6245,   0.8484},
                             {0.2893,   0.6288,   0.8437},
                             {0.2861,   0.6332,   0.8388},
                             {0.2828,   0.6376,   0.8338},
                             {0.2795,   0.6419,   0.8287},
                             {0.2760,   0.6462,   0.8234},
                             {0.2721,   0.6506,   0.8179},
                             {0.2679,   0.6550,   0.8123},
                             {0.2636,   0.6594,   0.8065},
                             {0.2590,   0.6638,   0.8006},
                             {0.2545,   0.6682,   0.7945},
                             {0.2499,   0.6726,   0.7884},
                             {0.2454,   0.6769,   0.7821},
                             {0.2411,   0.6812,   0.7758},
                             {0.2370,   0.6854,   0.7695},
                             {0.2332,   0.6896,   0.7632},
                             {0.2298,   0.6936,   0.7568},
                             {0.2268,   0.6976,   0.7505},
                             {0.2244,   0.7015,   0.7442},
                             {0.2226,   0.7052,   0.7380},
                             {0.2214,   0.7088,   0.7319},
                             {0.2210,   0.7123,   0.7258},
                             {0.2212,   0.7156,   0.7198},
                             {0.2216,   0.7188,   0.7139},
                             {0.2224,   0.7220,   0.7081},
                             {0.2233,   0.7250,   0.7022},
                             {0.2245,   0.7279,   0.6964},
                             {0.2259,   0.7308,   0.6906},
                             {0.2275,   0.7336,   0.6849},
                             {0.2292,   0.7363,   0.6791},
                             {0.2310,   0.7391,   0.6732},
                             {0.2330,   0.7418,   0.6674},
                             {0.2350,   0.7445,   0.6615},
                             {0.2370,   0.7472,   0.6556},
                             {0.2390,   0.7499,   0.6496},
                             {0.2411,   0.7527,   0.6435},
                             {0.2430,   0.7554,   0.6374},
                             {0.2450,   0.7583,   0.6311},
                             {0.2468,   0.7612,   0.6248},
                             {0.2486,   0.7642,   0.6183},
                             {0.2505,   0.7672,   0.6117},
                             {0.2525,   0.7703,   0.6050},
                             {0.2545,   0.7735,   0.5982},
                             {0.2566,   0.7766,   0.5913},
                             {0.2588,   0.7798,   0.5843},
                             {0.2609,   0.7830,   0.5772},
                             {0.2631,   0.7862,   0.5701},
                             {0.2654,   0.7893,   0.5630},
                             {0.2676,   0.7925,   0.5559},
                             {0.2699,   0.7956,   0.5487},
                             {0.2722,   0.7986,   0.5416},
                             {0.2744,   0.8015,   0.5346},
                             {0.2767,   0.8044,   0.5275},
                             {0.2789,   0.8072,   0.5206},
                             {0.2811,   0.8099,   0.5137},
                             {0.2833,   0.8125,   0.5069},
                             {0.2854,   0.8150,   0.5002},
                             {0.2876,   0.8174,   0.4935},
                             {0.2897,   0.8197,   0.4868},
                             {0.2918,   0.8219,   0.4801},
                             {0.2939,   0.8241,   0.4735},
                             {0.2960,   0.8262,   0.4668},
                             {0.2981,   0.8283,   0.4602},
                             {0.3003,   0.8304,   0.4536},
                             {0.3024,   0.8324,   0.4471},
                             {0.3045,   0.8344,   0.4405},
                             {0.3066,   0.8365,   0.4340},
                             {0.3088,   0.8385,   0.4276},
                             {0.3110,   0.8406,   0.4211},
                             {0.3131,   0.8427,   0.4147},
                             {0.3153,   0.8448,   0.4083},
                             {0.3176,   0.8470,   0.4019},
                             {0.3198,   0.8492,   0.3956},
                             {0.3220,   0.8515,   0.3890},
                             {0.3242,   0.8539,   0.3818},
                             {0.3263,   0.8564,   0.3742},
                             {0.3283,   0.8589,   0.3663},
                             {0.3304,   0.8614,   0.3582},
                             {0.3324,   0.8640,   0.3501},
                             {0.3345,   0.8666,   0.3420},
                             {0.3366,   0.8692,   0.3342},
                             {0.3388,   0.8717,   0.3266},
                             {0.3410,   0.8742,   0.3195},
                             {0.3433,   0.8767,   0.3129},
                             {0.3458,   0.8791,   0.3070},
                             {0.3483,   0.8814,   0.3019},
                             {0.3510,   0.8837,   0.2977},
                             {0.3539,   0.8858,   0.2946},
                             {0.3570,   0.8878,   0.2926},
                             {0.3602,   0.8896,   0.2919},
                             {0.3638,   0.8914,   0.2920},
                             {0.3677,   0.8931,   0.2921},
                             {0.3720,   0.8948,   0.2923},
                             {0.3766,   0.8964,   0.2927},
                             {0.3816,   0.8980,   0.2931},
                             {0.3868,   0.8996,   0.2936},
                             {0.3924,   0.9012,   0.2941},
                             {0.3981,   0.9026,   0.2947},
                             {0.4041,   0.9041,   0.2954},
                             {0.4102,   0.9055,   0.2961},
                             {0.4166,   0.9068,   0.2968},
                             {0.4231,   0.9081,   0.2976},
                             {0.4297,   0.9093,   0.2984},
                             {0.4364,   0.9105,   0.2992},
                             {0.4431,   0.9116,   0.3001},
                             {0.4500,   0.9126,   0.3009},
                             {0.4568,   0.9136,   0.3018},
                             {0.4639,   0.9145,   0.3028},
                             {0.4716,   0.9155,   0.3040},
                             {0.4797,   0.9165,   0.3054},
                             {0.4882,   0.9175,   0.3070},
                             {0.4970,   0.9184,   0.3087},
                             {0.5061,   0.9194,   0.3105},
                             {0.5154,   0.9203,   0.3124},
                             {0.5247,   0.9211,   0.3144},
                             {0.5342,   0.9219,   0.3164},
                             {0.5436,   0.9227,   0.3184},
                             {0.5529,   0.9234,   0.3203},
                             {0.5621,   0.9240,   0.3222},
                             {0.5710,   0.9245,   0.3239},
                             {0.5797,   0.9249,   0.3256},
                             {0.5880,   0.9252,   0.3271},
                             {0.5959,   0.9254,   0.3284},
                             {0.6033,   0.9255,   0.3295},
                             {0.6103,   0.9255,   0.3304},
                             {0.6172,   0.9255,   0.3313},
                             {0.6238,   0.9255,   0.3322},
                             {0.6303,   0.9255,   0.3330},
                             {0.6366,   0.9255,   0.3337},
                             {0.6428,   0.9255,   0.3344},
                             {0.6489,   0.9255,   0.3351},
                             {0.6549,   0.9255,   0.3358},
                             {0.6608,   0.9255,   0.3364},
                             {0.6666,   0.9255,   0.3370},
                             {0.6723,   0.9255,   0.3376},
                             {0.6781,   0.9255,   0.3382},
                             {0.6838,   0.9255,   0.3388},
                             {0.6895,   0.9255,   0.3394},
                             {0.6951,   0.9255,   0.3401},
                             {0.7009,   0.9255,   0.3407},
                             {0.7066,   0.9255,   0.3414},
                             {0.7124,   0.9255,   0.3421},
                             {0.7181,   0.9255,   0.3428},
                             {0.7238,   0.9255,   0.3435},
                             {0.7294,   0.9255,   0.3441},
                             {0.7351,   0.9255,   0.3448},
                             {0.7407,   0.9255,   0.3455},
                             {0.7462,   0.9255,   0.3462},
                             {0.7518,   0.9255,   0.3469},
                             {0.7573,   0.9255,   0.3475},
                             {0.7627,   0.9255,   0.3482},
                             {0.7681,   0.9255,   0.3489},
                             {0.7735,   0.9255,   0.3496},
                             {0.7789,   0.9255,   0.3502},
                             {0.7842,   0.9255,   0.3509},
                             {0.7895,   0.9255,   0.3516},
                             {0.7948,   0.9255,   0.3522},
                             {0.8000,   0.9255,   0.3529}
                         };
public:
    CubicYFColorMap();
    QColor cubicYFmap(int strt, int range, int min, int max, int v);
};

#endif // CUBICYFCOLORMAP_H


