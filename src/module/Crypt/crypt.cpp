#include "crypt.h"


#define WALKERSUN_ENCRPTY

SIP_ENCRYPT_TYPE sipEncryptFunctionSelect = SIP_ENCRYPT_UNKNOW;
RTP_ENCRYPT_TYPE rtpEncryptFunctionSelect = RTP_ENCRYPT_UNKNOW;

#define ACM_MAX 2147483647
static long acm_randseed;

void set_acm_seed(long seed);
long acm_random(void);
// Set the seed for the minimal standard random number generator.
void set_acm_seed(long seed)
{
	if( seed == 0 )
		seed = 1;
	if( seed < 0 )
		seed = -seed;
	acm_randseed = seed;
}

// This implements the "minimal standard random number generator"
// as proposed by Park and Miller in CACM October, 1988.
// It has a period of 2147483647.
long acm_random(void)
{
	long  lo, hi, test;
   
	hi = acm_randseed / 127773;
	lo = acm_randseed % 127773;
	test = 16807 * lo - 2836 * hi;
	if( test > 0 )
		acm_randseed = test;
	else
	acm_randseed = test + ACM_MAX;
	return(acm_randseed);   
}


#if 0	// little side
struct {
	unsigned int key[4];
} Secret_Data[]={
/*0*/{0xf98975e6,0x8569e099,0x6ceaf979,0xfb8e75ea},
/*1*/{0xef69e099,0x6ceaee79,0x8fb275e6,0x856996a2},
/*2*/{0x6ceaf979,0xfb8e75ea,0xef6996a2,0x6ce8f679},
/*3*/{0xd326759a,0xb769f6f5,0x6c85b979,0xf991758f},
/*4*/{0xe2699fe5,0x6ceaee79,0x9f99759a,0xb769f6f5},
/*5*/{0x6c813379,0x82a6758f,0xe26991e1,0x6ce8f679},
/*6*/{0xf58375ea,0xef69ca36,0x6c9aae79,0xefe5758b},
/*7*/{0x98698607,0x6c8fb179,0x8fa775fa,0xf769e6fa},
/*8*/{0x6c9a8579,0x9fb275fa,0xe069878d,0x6c9a0b79},
/*9*/{0x8aad75b8,0xa469f4fa,0x6cfd9e79,0xcf80758c},
/*10*/{0x936998f5,0x6ceaf979,0xffbf75ef,0xb469fbc9},
/*11*/{0x6c8aae79,0xefe57594,0xe0698307,0x6c9ffb79},
/*12*/{0x9cb775fa,0xe06985a7,0x6c9ffb79,0x9f9f7589},
/*13*/{0xb269f6f5,0x6cf8df79,0x8fa7758f,0xe269bc0d},
/*14*/{0x6ce8f679,0x80a57596,0x8e69e88d,0x6c9fff79},
/*15*/{0x8f89758f,0xe269cbb0,0x6c8a9579,0xefe57592},
/*16*/{0xf569c5e3,0x6c9b8479,0xeff275e4,0x986983f3},
/*17*/{0x6c809379,0x9ae775c7,0xa5699c9f,0x6ceaf979},
/*18*/{0x87f075eb,0xf7699c9f,0x6c9b8479,0xedea75e0},
/*19*/{0x86698689,0x6cd63a79,0x83a97590,0xb669f6f5},
/*20*/{0x6ca22c79,0x9e9e7596,0xac69e690,0x6ceaf979},
/*21*/{0xa50575ef,0x80699ab9,0x6c999b79,0xefe575e1},
/*22*/{0x93699a9b,0x6c86b579,0xcf0875fa,0xe06987ad},
/*23*/{0x6c8bc079,0x83a975e3,0x8169f6f5,0x6c8ee579},
/*24*/{0xada87596,0xac69be0a,0x6ce8f679,0x868e758b},
/*25*/{0x98699724,0x6c819e79,0x80c775c6,0x2369cbb0},
/*26*/{0x6c9ffb79,0x868b75fa,0xe0698589,0x6cfbee79},
/*27*/{0x9d9d758f,0xe269e895,0x6ceaf979,0x842f7597},
/*28*/{0xa36982ae,0x6c989c79,0xfab175eb,0xf7698b36},
/*29*/{0x6ceaf979,0x85b375ef,0xb469e7e2,0x6c9a8579},
/*30*/{0xefe575c7,0xa569e3a1,0x6cfbee79,0x869875fa},
/*31*/{0xe069ecff,0x6cfa9c79,0xfab175e1,0xe969eb8a},
/*32*/{0x6ce8f679,0xfbbb7597,0xe569edfc,0x6cf78679},
/*33*/{0xefe57593,0x8b69878d,0x6cfbee79,0x84ec75f8},
/*34*/{0xef69e7e2,0x6c808579,0x953d75fa,0xe0699fe0},
/*35*/{0x6c8aa479,0xfef275f9,0xc269f6e2,0x6cfbee79},
/*36*/{0x950f75b2,0x3569e09a,0x6c9ffb79,0x94d275fa},
/*37*/{0xe0699fe0,0x6c8aa479,0xfef275c7,0xa569e410},
/*38*/{0x6ceaee79,0xfef2758b,0xc669ea90,0x6c9aae79},
/*39*/{0xefe57593,0xf569859d,0x6cfbee79,0xcd3075f8},
/*40*/{0xef699f9e,0x6c9b8179,0x8e347591,0x876983f7},
/*41*/{0x6c9f9b79,0xefe57595,0xdf6993bd,0x6c999179},
/*42*/{0xefe575d5,0xc16993bd,0x6cf1ec79,0xeff27591},
/*43*/{0xb96993bd,0x6c9feb79,0xefe575d6,0xc66993bd},
/*44*/{0x6cf09279,0xedea75ea,0xef699fe0,0x6c8aa479},
/*45*/{0xd326758f,0xe669ca36,0x6c9aae79,0xefe57593},
/*46*/{0xf569e2ab,0x6c9fff79,0x999675eb,0xf769ed8b},
/*47*/{0x6cd7bc79,0x9efb75f8,0xef69cbb0,0x6cd63a79},
/*48*/{0xd2a075fa,0xe069d12d,0x6cd63a79,0xfef2758f},
/*49*/{0x8269f4fa,0x6cfc9579,0xd71675ef,0xb469869a},
/*50*/{0x6c9ffb79,0xf7bb75eb,0xf76986f6,0x6ceaf979},
/*51*/{0xd0107580,0xae699e9f,0x6c813379,0x82a6758f},
/*52*/{0xe2698283,0x6ce8f679,0xf8a5759f,0xa869b113},
/*53*/{0x6ceaf979,0xf1ab759f,0xa869e9d3,0x6ceaf979},
/*54*/{0xf684759f,0xa869ecbb,0x6ceaf979,0x81e5759f},
/*55*/{0xa869dc34,0x6ce8f679,0x99f67580,0xae6993bd},
/*56*/{0x6c81a179,0xf8af75fa,0xe0699bb8,0x6cfbee79},
/*57*/{0x9ae3758c,0x206983f7,0x6c9e8679,0xefe57596},
/*58*/{0xbf69e082,0x6c9ffb79,0x838175f8,0xef6999b5},
/*59*/{0x6cfc8d79,0xfef27591,0x8669f6f5,0x6c9b8179},
/*60*/{0x842f7597,0xa369cbb0,0x6c8f3679,0xf5fe75fa},
/*61*/{0xf7699724,0x6c819e79,0xfef27591,0x8669f6f5},
/*62*/{0x6c9b8179,0xfc907589,0x9969cbb0,0x6c8f3679},
/*63*/{0xf5fe75f8,0xef6999b5,0x6cfc8d79,0x9ae775b0},
/*64*/{0x2b69f6f5,0x6c8fb179,0xe63875b0,0x9e69ee38},
/*65*/{0x6cf3a079,0xeff67595,0xdf69e3a1,0x6cfbee79},
/*66*/{0x8bb575fa,0xe069d18a,0x6cffad79,0x9fb375ea},
/*67*/{0xba69f4fa,0x6cffb579,0x9d9d75cd,0xf969ed1f},
/*68*/{0x6ceaf979,0xfef27591,0xab699f81,0x6c9f8579},
/*69*/{0xedea75e0,0x84699ca8,0x6cfbee79,0x878975fa},
/*70*/{0xe0699f9e,0x6c9f0b79,0x9cb775b3,0xbe69f6f5},
/*71*/{0x6c99ab79,0xa6bb758f,0xe269bc0d,0x6ceaf979},
/*72*/{0x868e758f,0x126999b5,0x6cfc8d79,0xf4b075f8},
/*73*/{0xef69eb3d,0x6cf73179,0x84bd75ed,0xaa69f6f5},
/*74*/{0x6c9a9679,0x9ae775eb,0xf7699283,0x6ce8f679},
/*75*/{0x80a575b0,0x0069e081,0x6cf79679,0xedea7595},
/*76*/{0xa069e081,0x6c82ac79,0x9e9d759d,0x9069bc15},
/*77*/{0x6c8e8779,0xf28a758c,0x9369f6f5,0x6c9b8179},
/*78*/{0x8aad75eb,0xf769828d,0x6c80af79,0xefe575e0},
/*79*/{0x86699185,0x6ca01979,0x85b375f8,0xef699f9e},
/*80*/{0x6c9b8179,0x8e347591,0x8769b4b8,0x6c8fb179},
/*81*/{0x85a475ef,0xb4699cb4,0x6c869d79,0xefe57594},
/*82*/{0xae6993bd,0x6c80b879,0xfab17590,0xa169e1bf},
/*83*/{0x6ce8f679,0xfb8e758b,0x986993bd,0x6cd63a79},
/*84*/{0x87f4758b,0xe269f6e6,0x6cf09f79,0x889575ea},
/*85*/{0x856993bd,0x6c82e879,0xedea7590,0x83699c9f},
/*86*/{0x6c81a179,0x87e775f8,0xef699ef7,0x6c89ae79},
/*87*/{0x842f7597,0xa369e3a1,0x6cfbee79,0xecc775fa},
/*88*/{0xe06999d7,0x6cf8df79,0x84827592,0xb569c5e3},
/*89*/{0x6ceaf979,0xf58375d7,0x9b69b9ef,0x6cfc9579},
/*90*/{0x9e9a75f8,0xef69eb8a,0x6c809379,0xf99175fa},
/*91*/{0xe069859d,0x6c809379,0xd01075fa,0xe069903a},
/*92*/{0x6c809379,0x848375fa,0xe069848d,0x6c809379},
/*93*/{0x9c8c75fa,0xe06980a7,0x6c809379,0x9a8775fa},
/*94*/{0xe0699f9b,0x6c809379,0x889575fa,0xe069d18a},
/*95*/{0x6c809379,0xd93b75f8,0xef69e2ab,0x6c87fc79},
/*96*/{0xfef275f9,0xc269f6f5,0x6cf09f79,0xd326758a},
/*97*/{0x8469f4fa,0x6cfa8379,0xfab1758a,0xe36983f7},
/*98*/{0x6ceaf979,0xfef27591,0xab6993bd,0x6c9b8479},
/*99*/{0xedea75ed,0xeb69e3a1,0x6cad1f79,0x9ae775fa},
/*100*/{0xe069e7e2,0x6cf69c79,0xa50575e8,0xef69f4fa},
/*101*/{0x6cf4a579,0x9fb875c4,0x0169999a,0x6ceaf979},
/*102*/{0x88e3758f,0xe2699185,0x6c838d79,0xedea75e1},
/*103*/{0xf7698c1f,0x6cffad79,0xbed575fa,0xe069828d},
/*104*/{0x6c962f79,0x8aad75e7,0x8069f4fa,0x6cf0f379},
/*105*/{0xff80759a,0xb7699eb5,0x6ceaf994,0x876999b5},
/*106*/{0x6c9ffb79,0xf98975f8,0xef69883d,0x6ce91c79},
/*107*/{0x8a8e75e8,0xeb6987e2,0x6ceaf979,0x889575c6},
/*108*/{0x2369be21,0x6cf3a079,0xeff675d5,0xef69cfbb},
/*109*/{0x6c9f9779,0x84a875fa,0xe0699185,0x6c81b279},
/*110*/{0xc7a275da,0xf669efac,0x6ceaea79,0xd0a575ea},
/*111*/{0xb16985a7,0x6c9a0e79,0xefe5759d,0x9069ca36},
/*112*/{0x6cfd9679,0xf6bc75fa,0xf369c582,0x6c8aa479},
/*113*/{0x9a8775de,0xb469f6f5,0x6c8d8979,0xd32675c7},
/*114*/{0xa569efac,0x6ceaea79,0x80a575b0,0x1869bc06},
/*115*/{0x6ca31d79,0xefe5759d,0x9069ca36,0x6cfd9379},
/*116*/{0xf6bc75fa,0xf36996ae,0x6cf98279,0x878d7586},
/*117*/{0x1369f6f5,0x6c8d8979,0xd326758f,0xe669efac},
/*118*/{0x6ceaea79,0x85b3758f,0xe26985aa,0x6c9ffb79},
/*119*/{0xedeb7590,0xb669e3a1,0x6cfbee79,0x9f9975fa},
/*120*/{0xe069cbb0,0x6cffad79,0xfef27593,0x9d69f6f5},
/*121*/{0x6ca01979,0xfab175eb,0xf769818e,0x6ceaf979},
/*122*/{0x868e758f,0x126985a7,0x6cfc9779,0xedea7591},
/*123*/{0xb1699ff7,0x6c94c279,0xf5fb758b,0xf76988c7},
/*124*/{0x6ceaf979,0xe2dd759f,0xa869ca36,0x6ceaf979},
/*125*/{0x9f997585,0xcb6983f7,0x6c9a9679,0xedea7582},
/*126*/{0xa3698ea3,0x6c9b8179,0xd2a0759f,0xbb69f6f5},
/*127*/{0x6ce7c179,0x8aad75c6,0x2369f6f5,0x6c9a8579},
/*128*/{0x8abe758f,0xe269869a,0x6ce8f679,0xa4c675c8},
/*129*/{0xb869bee2,0x6c9b8179,0xd2a07593,0x9e69f6f5},
/*130*/{0x6ce7c179,0x8aad75c6,0x2369f6f5,0x6c9a8579},
/*131*/{0x869b758f,0xe269869a,0x6ce8f679,0xf583758a},
/*132*/{0x9c6983f7,0x6c9b8179,0xd2a07599,0xb769f6f5},
/*133*/{0x6cd63a79,0x9ae7758b,0x9869cbb0,0x6c9a9679},
/*134*/{0xedea7597,0xa9699cf2,0x6c88bb79,0x8482759d},
/*135*/{0xf36996fd,0x6ceaf979,0x82ac758b,0xb86994b7},
/*136*/{0x6c819e79,0xfab3759a,0x0c69f6f5,0x6c87b079},
/*137*/{0x82ff7598,0xa2699d92,0x6cf68f79,0x87e575fa},
/*138*/{0xe069a400,0x6cbb1279,0x80a675f2,0x0f6994b7},
/*139*/{0x6c819e79,0x9c8d75e9,0x2069eaa8,0x6ceaf979},
/*140*/{0xa73075ec,0x8f6983f7,0x6c91ce79,0x8da77591},
/*141*/{0x87698589,0x6cfe9479,0xedea7593,0x8b69878d},
/*142*/{0x6c8b2879,0x848275c7,0xa569ede0,0x6cfbee79},
/*143*/{0xd2a0759d,0xf369f6f5,0x6cf09f79,0x84ec75e8},
/*144*/{0x87699df8,0x6cfd9e79,0xedea75d5,0xd5699db1},
/*145*/{0x6c81a179,0xbf0975fa,0xe0698c1f,0x6cfda679},
/*146*/{0xf7f57591,0xb8699cb4,0x6ce8f679,0xf687758f},
/*147*/{0x1269d9c0,0x6c81bd79,0x84bd75aa,0x0c69f6e6},
/*148*/{0x6cc5cc79,0xefe575c7,0xa5699a9b,0x6cfc9679},
/*149*/{0x9ae77591,0xb869a619,0x6ceaf979,0x86ee758f},
/*150*/{0xe2699dad,0x6cba1579,0xefe57593,0x8b698307},
/*151*/{0x6cc5cc79,0x84a17591,0xb869a619,0x6ce8f679},
/*152*/{0xf687758f,0x12698c1f,0x6cfda679,0xf7f57591},
/*153*/{0xb8699cb4,0x6ceaea79,0x82a87592,0xb569878d},
/*154*/{0x6c9a8579,0xf8ba75e2,0xf0698086,0x6ceaf979},
/*155*/{0xd2a07597,0xad698689,0x6c80b879,0xefe575e5},
/*156*/{0xfc699bb8,0x6cd63a79,0x85a475fa,0xe0699bb8},
/*157*/{0x6c9a8579,0xf68775e2,0xf069f6e6,0x6cf09f79},
/*158*/{0x950f758b,0x98699cb4,0x6cd7bc79,0x80a57596},
/*159*/{0x8e698086,0x6ceaf979,0xc83d75e6,0x8569878d},
/*160*/{0x6cf59179,0xa0ff7595,0xa0699a9b,0x6ceaee79},
/*161*/{0xdc92758b,0x98699cb4,0x6cd7bc79,0x80a57596},
/*162*/{0x8e698086,0x6ceaf979,0x888275e6,0x8569878d},
/*163*/{0x6c9acf79,0xa0ff7595,0xa0699a9b,0x6ce8f679},
/*164*/{0x9edc758f,0xe269e7e2,0x6c9bdf79,0x8fb2758d},
/*165*/{0xf769879d,0x6ceaf979,0x8ee9758f,0xe269e7e2},
/*166*/{0x6c8bd179,0x8fb2758d,0xf7699afa,0x6ce8f679},
/*167*/{0xfee2758f,0xe269e7e2,0x6cfc9679,0x8fb2758d},
/*168*/{0xf7699bfb,0x6ce8f679,0xf8827591,0xb1698086},
/*169*/{0x6cfbee79,0xf380758f,0x8e6981c8,0x6ceaf979},
/*170*/{0xf58375e2,0xa069e3a1,0x6cd7bc79,0x9ef275f8},
/*171*/{0xef6993bd,0x6c809a79,0xfef275e9,0xc569f6f5},
/*172*/{0x6c8fb179,0x838b75eb,0xf7699698,0x6ceaf979},
/*173*/{0x8c027599,0x07698cbb,0x6cfbee79,0xf380759a},
/*174*/{0xb769f6f5,0x6cc69879,0xd63e75b5,0xfa69ca36},
/*175*/{0x6c87ba79,0xedea7593,0x8b698307,0x6cd63a79},
/*176*/{0xeca7758f,0xe269f5b7,0x6ceaf979,0xd3267596},
/*177*/{0xbf6983f7,0x6c86a679,0xefe57593,0x8b698307},
/*178*/{0x6caae479,0xf79975f8,0xef6986f4,0x6c9ffb79},
/*179*/{0xfef2758b,0xc66993bd,0x6c838279,0xefe575b2},
/*180*/{0x1f6983f7,0x6cfbee79,0x9ec3759f,0xa869b4b8},
/*181*/{0x6ce8f679,0xc48575e0,0x896983f7,0x6cfc9579},
/*182*/{0xefe5758b,0x986986a0,0x6cf4a479,0x9ae7758a},
/*183*/{0x9c69f6f5,0x6c8d8979,0x9ae375e0,0x89699fe5},
/*184*/{0x6ceaf979,0x868e759a,0xb769e099,0x6cf53a79},
/*185*/{0xedea75e0,0x896983f7,0x6c809379,0xd2a07593},
/*186*/{0xf3698086,0x6ceaf979,0x82eb759a,0xa26985a7},
/*187*/{0x6c84fd79,0xefe57590,0xa269e7e2,0x6cf69c79},
/*188*/{0x9b1b75f8,0xef69e2ab,0x6c87fc79,0xfef275e6},
/*189*/{0x8569820b,0x6ceaf979,0xf58375d6,0xc669cbb0},
/*190*/{0x6c9ffb79,0x849475f8,0xef6981fc,0x6c90b779},
/*191*/{0x84bd75ef,0xe0699cbf,0x6cfdfd79,0xefe575f3},
/*192*/{0x3d698cbb,0x6c81a179,0x82fe7592,0x8869b5d6},
/*193*/{0x6ceaf979,0xcff97580,0xae6993bd,0x6c81a179},
/*194*/{0xf38475fa,0xe069c936,0x6c90b779,0x84bd75e8},
/*195*/{0xb56983f7,0x6cc5f779,0xadc575fa,0xe0699fb2},
/*196*/{0x6c90b779,0x8aad7591,0xb869cdba,0x6ceaf979},
/*197*/{0xd9ae7580,0xae6993bd,0x6c81a179,0xf58175fa},
/*198*/{0xe069c9df,0x6c90b779,0x8aad7591,0xb869c8b8},
/*199*/{0x6ce8f679,0x86a2759d,0x9069c8b8,0x6c9b8179},
/*200*/{0xff8775b5,0x236983f7,0x6c99b979,0x8bac75fa},
/*201*/{0xf3699fb2,0x6c8d8979,0xfcfb758b,0x9869eb9a},
/*202*/{0x6ccd8679,0x9ae77589,0xa0699ca3,0x6ceaea79},
/*203*/{0xfdea75ed,0x8769e099,0x6c9c8a79,0xfef2758a},
/*204*/{0xb76986f6,0x6ceaf979,0xfbbb7597,0xe569e7e2},
/*205*/{0x6c9afa79,0xefe575e0,0x86699185,0x6cf88379},
/*206*/{0xfef27589,0x8e69e690,0x6ce8f679,0x9a8b7595},
/*207*/{0xdf69cd16,0x6ceaf979,0x869175b5,0x2369ef0f},
/*208*/{0x6ceaf979,0x842f7597,0xa369d412,0x6c9ea279},
/*209*/{0xefe57597,0xad69878d,0x6c9a0b79,0x8aad75d6},
/*210*/{0x8169f4fa,0x6cfea779,0x82a67585,0xf36989e6},
/*211*/{0x6ceaf979,0xf4be75c3,0x3b6993bd,0x6cf1ac79},
/*212*/{0xedea75c3,0x3b69eda0,0x6c9dee79,0xa02675fa},
/*213*/{0xe069b936,0x6c9dee79,0xc384759a,0xb069f4fa},
/*214*/{0x6cc69879,0x8fb5758d,0xf769e6fa,0x6ceaf979},
/*215*/{0x9ae375ea,0xef6981e2,0x6c8aa279,0xefe575eb},
/*216*/{0xf76983f3,0x6cfaf679,0xefe57594,0xb16982ae},
/*217*/{0x6ceaf979,0x9c9e75f8,0xef6983f3,0x6cfaf679},
/*218*/{0x849475fa,0xe0699d84,0x6c8d9e79,0xf5e275fa},
/*219*/{0xe069ecf2,0x6c8d9e79,0x81bc75fa,0xe06998ac},
/*220*/{0x6c8d9e79,0x80a575fa,0xe06999b5,0x6c8d9e79},
/*221*/{0xf98975fa,0xe069e099,0x6c8d9e79,0xf28a75f8},
/*222*/{0xef69ce04,0x6c80b879,0xfef275ed,0xb569f4fa},
/*223*/{0x6c85fe79,0x858675fa,0xe0699a9b,0x6c9fff79},
/*224*/{0x9f99758f,0xe269f4fa,0x6c8fb179,0xf88775fa},
/*225*/{0xe0698611,0x6cffad79,0x9b3c758f,0xe269f4fa},
/*226*/{0x6c8fb179,0xf88775fa,0xe0699bee,0x6c9ffb79},
/*227*/{0xedea759f,0xa869e197,0x6ceaf979,0x82a3758f},
/*228*/{0xe269f4fa,0x6c999079,0xfef2758e,0xaf697588},
/*229*/{0x85eaf979,0x9f9975eb,0xf769859c,0x6c989c79},
/*230*/{0xedea75f3,0x3d698cbb,0x6c8fb179,0x950f7588},
/*231*/{0x9869f4fa,0x6cf0f379,0xff807593,0x8e699eb5},
/*232*/{0x6ceaf979,0xfc907589,0x9969e88d,0x6c9f0b79},
/*233*/{0x829b758e,0x98699de2,0x6ce8f679,0xf8ba75ec},
/*234*/{0x8c69da0c,0x6ceaf979,0x9f997591,0x86699421},
/*235*/{0x6ceaee79,0x9a8e75e2,0x9769e6af,0x6ceaf979},
/*236*/{0x9f9975ed,0xbf69d797,0x6ceaee79,0x8db0758a},
/*237*/{0x0469e7e2,0x6cf39879,0xefe5758a,0x9c6985fb},
/*238*/{0x6cfd9d79,0xeff275de,0xb469e98b,0x6cf2bd79},
/*239*/{0xcd3075fa,0xe0698689,0x6c9f8779,0xfff575f8},
/*240*/{0xef69e807,0x6c8b2879,0xdbc0758f,0x8b69f6f5},
/*241*/{0x6c8aa479,0x8cb275e9,0x9569e4ef,0x6ceaee79},
/*242*/{0xf1177591,0x8669c2d0,0x6c882d79,0xefe5759a},
/*243*/{0xbd69da94,0x6c99f779,0xf88175fa,0xf769e807},
/*244*/{0x6c8e9c79,0xdbc07599,0xb769f6f5,0x6cf81c79},
/*245*/{0x951575c6,0x23698689,0x6ce8f679,0xf8827591},
/*246*/{0xb1698086,0x6ceaf979,0x9e9d75c7,0xa5699b9d},
/*247*/{0x6cfbee79,0x9baa75fa,0xe069ec93,0x6c88bb79},
/*248*/{0x9f997592,0xb569d980,0x6ceaf979,0x9ec37592},
/*249*/{0x9469e4fe,0x6cd1b679,0xefe57590,0x95699ee4},
/*250*/{0x6cf08479,0x9fb275f8,0xef69e807,0x6cc50279},
/*251*/{0xd32675c8,0x2269f4fa,0x6c87fd79,0x9ae7759c},
/*252*/{0x2f69e5fb,0x6ceaf979,0x83a97591,0xe969db8e},
/*253*/{0x6cf39b79,0xeff67590,0x8a6983f7,0x6c8c3679},
/*254*/{0xdcf375fa,0xe0699ab9,0x6c81f079,0xf6877591},
/*255*/{0xb869f6e6,0x6c819e79,0x9ae77592,0xb5699bee}};
#else
struct SecretData_s{
	unsigned int key[4];
	}
	Secret_Data[]={
/*0*/{0xe67589f9,0x99e06985,0x79f9ea6c,0xea758efb},
/*1*/{0x99e069ef,0x79eeea6c,0xe675b28f,0xa2966985},
/*2*/{0x79f9ea6c,0xea758efb,0xa29669ef,0x79f6e86c},
/*3*/{0x9a7526d3,0xf5f669b7,0x79b9856c,0x8f7591f9},
/*4*/{0xe59f69e2,0x79eeea6c,0x9a75999f,0xf5f669b7},
/*5*/{0x7933816c,0x8f75a682,0xe19169e2,0x79f6e86c},
/*6*/{0xea7583f5,0x36ca69ef,0x79ae9a6c,0x8b75e5ef},
/*7*/{0x07866998,0x79b18f6c,0xfa75a78f,0xfae669f7},
/*8*/{0x79859a6c,0xfa75b29f,0x8d8769e0,0x790b9a6c},
/*9*/{0xb875ad8a,0xfaf469a4,0x799efd6c,0x8c7580cf},
/*10*/{0xf5986993,0x79f9ea6c,0xef75bfff,0xc9fb69b4},
/*11*/{0x79ae8a6c,0x9475e5ef,0x078369e0,0x79fb9f6c},
/*12*/{0xfa75b79c,0xa78569e0,0x79fb9f6c,0x89759f9f},
/*13*/{0xf5f669b2,0x79dff86c,0x8f75a78f,0x0dbc69e2},
/*14*/{0x79f6e86c,0x9675a580,0x8de8698e,0x79ff9f6c},
/*15*/{0x8f75898f,0xb0cb69e2,0x79958a6c,0x9275e5ef},
/*16*/{0xe3c569f5,0x79849b6c,0xe475f2ef,0xf3836998},
/*17*/{0x7993806c,0xc775e79a,0x9f9c69a5,0x79f9ea6c},
/*18*/{0xeb75f087,0x9f9c69f7,0x79849b6c,0xe075eaed},
/*19*/{0x89866986,0x793ad66c,0x9075a983,0xf5f669b6},
/*20*/{0x792ca26c,0x96759e9e,0x90e669ac,0x79f9ea6c},
/*21*/{0xef7505a5,0xb99a6980,0x799b996c,0xe175e5ef},
/*22*/{0x9b9a6993,0x79b5866c,0xfa7508cf,0xad8769e0},
/*23*/{0x79c08b6c,0xe375a983,0xf5f66981,0x79e58e6c},
/*24*/{0x9675a8ad,0x0abe69ac,0x79f6e86c,0x8b758e86},
/*25*/{0x24976998,0x799e816c,0xc675c780,0xb0cb6923},
/*26*/{0x79fb9f6c,0xfa758b86,0x898569e0,0x79eefb6c},
/*27*/{0x8f759d9d,0x95e869e2,0x79f9ea6c,0x97752f84},
/*28*/{0xae8269a3,0x799c986c,0xeb75b1fa,0x368b69f7},
/*29*/{0x79f9ea6c,0xef75b385,0xe2e769b4,0x79859a6c},
/*30*/{0xc775e5ef,0xa1e369a5,0x79eefb6c,0xfa759886},
/*31*/{0xffec69e0,0x799cfa6c,0xe175b1fa,0x8aeb69e9},
/*32*/{0x79f6e86c,0x9775bbfb,0xfced69e5,0x7986f76c},
/*33*/{0x9375e5ef,0x8d87698b,0x79eefb6c,0xf875ec84},
/*34*/{0xe2e769ef,0x7985806c,0xfa753d95,0xe09f69e0},
/*35*/{0x79a48a6c,0xf975f2fe,0xe2f669c2,0x79eefb6c},
/*36*/{0xb2750f95,0x9ae06935,0x79fb9f6c,0xfa75d294},
/*37*/{0xe09f69e0,0x79a48a6c,0xc775f2fe,0x10e469a5},
/*38*/{0x79eeea6c,0x8b75f2fe,0x90ea69c6,0x79ae9a6c},
/*39*/{0x9375e5ef,0x9d8569f5,0x79eefb6c,0xf87530cd},
/*40*/{0x9e9f69ef,0x79819b6c,0x9175348e,0xf7836987},
/*41*/{0x799b9f6c,0x9575e5ef,0xbd9369df,0x7991996c},
/*42*/{0xd575e5ef,0xbd9369c1,0x79ecf16c,0x9175f2ef},
/*43*/{0xbd9369b9,0x79eb9f6c,0xd675e5ef,0xbd9369c6},
/*44*/{0x7992f06c,0xea75eaed,0xe09f69ef,0x79a48a6c},
/*45*/{0x8f7526d3,0x36ca69e6,0x79ae9a6c,0x9375e5ef},
/*46*/{0xabe269f5,0x79ff9f6c,0xeb759699,0x8bed69f7},
/*47*/{0x79bcd76c,0xf875fb9e,0xb0cb69ef,0x793ad66c},
/*48*/{0xfa75a0d2,0x2dd169e0,0x793ad66c,0x8f75f2fe},
/*49*/{0xfaf46982,0x7995fc6c,0xef7516d7,0x9a8669b4},
/*50*/{0x79fb9f6c,0xeb75bbf7,0xf68669f7,0x79f9ea6c},
/*51*/{0x807510d0,0x9f9e69ae,0x7933816c,0x8f75a682},
/*52*/{0x838269e2,0x79f6e86c,0x9f75a5f8,0x13b169a8},
/*53*/{0x79f9ea6c,0x9f75abf1,0xd3e969a8,0x79f9ea6c},
/*54*/{0x9f7584f6,0xbbec69a8,0x79f9ea6c,0x9f75e581},
/*55*/{0x34dc69a8,0x79f6e86c,0x8075f699,0xbd9369ae},
/*56*/{0x79a1816c,0xfa75aff8,0xb89b69e0,0x79eefb6c},
/*57*/{0x8c75e39a,0xf7836920,0x79869e6c,0x9675e5ef},
/*58*/{0x82e069bf,0x79fb9f6c,0xf8758183,0xb59969ef},
/*59*/{0x798dfc6c,0x9175f2fe,0xf5f66986,0x79819b6c},
/*60*/{0x97752f84,0xb0cb69a3,0x79368f6c,0xfa75fef5},
/*61*/{0x249769f7,0x799e816c,0x9175f2fe,0xf5f66986},
/*62*/{0x79819b6c,0x897590fc,0xb0cb6999,0x79368f6c},
/*63*/{0xf875fef5,0xb59969ef,0x798dfc6c,0xb075e79a},
/*64*/{0xf5f6692b,0x79b18f6c,0xb07538e6,0x38ee699e},
/*65*/{0x79a0f36c,0x9575f6ef,0xa1e369df,0x79eefb6c},
/*66*/{0xfa75b58b,0x8ad169e0,0x79adff6c,0xea75b39f},
/*67*/{0xfaf469ba,0x79b5ff6c,0xcd759d9d,0x1fed69f9},
/*68*/{0x79f9ea6c,0x9175f2fe,0x819f69ab,0x79859f6c},
/*69*/{0xe075eaed,0xa89c6984,0x79eefb6c,0xfa758987},
/*70*/{0x9e9f69e0,0x790b9f6c,0xb375b79c,0xf5f669be},
/*71*/{0x79ab996c,0x8f75bba6,0x0dbc69e2,0x79f9ea6c},
/*72*/{0x8f758e86,0xb5996912,0x798dfc6c,0xf875b0f4},
/*73*/{0x3deb69ef,0x7931f76c,0xed75bd84,0xf5f669aa},
/*74*/{0x79969a6c,0xeb75e79a,0x839269f7,0x79f6e86c},
/*75*/{0xb075a580,0x81e06900,0x7996f76c,0x9575eaed},
/*76*/{0x81e069a0,0x79ac826c,0x9d759d9e,0x15bc6990},
/*77*/{0x79878e6c,0x8c758af2,0xf5f66993,0x79819b6c},
/*78*/{0xeb75ad8a,0x8d8269f7,0x79af806c,0xe075e5ef},
/*79*/{0x85916986,0x7919a06c,0xf875b385,0x9e9f69ef},
/*80*/{0x79819b6c,0x9175348e,0xb8b46987,0x79b18f6c},
/*81*/{0xef75a485,0xb49c69b4,0x799d866c,0x9475e5ef},
/*82*/{0xbd9369ae,0x79b8806c,0x9075b1fa,0xbfe169a1},
/*83*/{0x79f6e86c,0x8b758efb,0xbd936998,0x793ad66c},
/*84*/{0x8b75f487,0xe6f669e2,0x799ff06c,0xea759588},
/*85*/{0xbd936985,0x79e8826c,0x9075eaed,0x9f9c6983},
/*86*/{0x79a1816c,0xf875e787,0xf79e69ef,0x79ae896c},
/*87*/{0x97752f84,0xa1e369a3,0x79eefb6c,0xfa75c7ec},
/*88*/{0xd79969e0,0x79dff86c,0x92758284,0xe3c569b5},
/*89*/{0x79f9ea6c,0xd77583f5,0xefb9699b,0x7995fc6c},
/*90*/{0xf8759a9e,0x8aeb69ef,0x7993806c,0xfa7591f9},
/*91*/{0x9d8569e0,0x7993806c,0xfa7510d0,0x3a9069e0},
/*92*/{0x7993806c,0xfa758384,0x8d8469e0,0x7993806c},
/*93*/{0xfa758c9c,0xa78069e0,0x7993806c,0xfa75879a},
/*94*/{0x9b9f69e0,0x7993806c,0xfa759588,0x8ad169e0},
/*95*/{0x7993806c,0xf8753bd9,0xabe269ef,0x79fc876c},
/*96*/{0xf975f2fe,0xf5f669c2,0x799ff06c,0x8a7526d3},
/*97*/{0xfaf46984,0x7983fa6c,0x8a75b1fa,0xf78369e3},
/*98*/{0x79f9ea6c,0x9175f2fe,0xbd9369ab,0x79849b6c},
/*99*/{0xed75eaed,0xa1e369eb,0x791fad6c,0xfa75e79a},
/*100*/{0xe2e769e0,0x799cf66c,0xe87505a5,0xfaf469ef},
/*101*/{0x79a5f46c,0xc475b89f,0x9a996901,0x79f9ea6c},
/*102*/{0x8f75e388,0x859169e2,0x798d836c,0xe175eaed},
/*103*/{0x1f8c69f7,0x79adff6c,0xfa75d5be,0x8d8269e0},
/*104*/{0x792f966c,0xe775ad8a,0xfaf46980,0x79f3f06c},
/*105*/{0x9a7580ff,0xb59e69b7,0x94f9ea6c,0xb5996987},
/*106*/{0x79fb9f6c,0xf87589f9,0x3d8869ef,0x791ce96c},
/*107*/{0xe8758e8a,0xe28769eb,0x79f9ea6c,0xc6759588},
/*108*/{0x21be6923,0x79a0f36c,0xd575f6ef,0xbbcf69ef},
/*109*/{0x79979f6c,0xfa75a884,0x859169e0,0x79b2816c},
/*110*/{0xda75a2c7,0xacef69f6,0x79eaea6c,0xea75a5d0},
/*111*/{0xa78569b1,0x790e9a6c,0x9d75e5ef,0x36ca6990},
/*112*/{0x7996fd6c,0xfa75bcf6,0x82c569f3,0x79a48a6c},
/*113*/{0xde75879a,0xf5f669b4,0x79898d6c,0xc77526d3},
/*114*/{0xacef69a5,0x79eaea6c,0xb075a580,0x06bc6918},
/*115*/{0x791da36c,0x9d75e5ef,0x36ca6990,0x7993fd6c},
/*116*/{0xfa75bcf6,0xae9669f3,0x7982f96c,0x86758d87},
/*117*/{0xf5f66913,0x79898d6c,0x8f7526d3,0xacef69e6},
/*118*/{0x79eaea6c,0x8f75b385,0xaa8569e2,0x79fb9f6c},
/*119*/{0x9075ebed,0xa1e369b6,0x79eefb6c,0xfa75999f},
/*120*/{0xb0cb69e0,0x79adff6c,0x9375f2fe,0xf5f6699d},
/*121*/{0x7919a06c,0xeb75b1fa,0x8e8169f7,0x79f9ea6c},
/*122*/{0x8f758e86,0xa7856912,0x7997fc6c,0x9175eaed},
/*123*/{0xf79f69b1,0x79c2946c,0x8b75fbf5,0xc78869f7},
/*124*/{0x79f9ea6c,0x9f75dde2,0x36ca69a8,0x79f9ea6c},
/*125*/{0x8575999f,0xf78369cb,0x79969a6c,0x8275eaed},
/*126*/{0xa38e69a3,0x79819b6c,0x9f75a0d2,0xf5f669bb},
/*127*/{0x79c1e76c,0xc675ad8a,0xf5f66923,0x79859a6c},
/*128*/{0x8f75be8a,0x9a8669e2,0x79f6e86c,0xc875c6a4},
/*129*/{0xe2be69b8,0x79819b6c,0x9375a0d2,0xf5f6699e},
/*130*/{0x79c1e76c,0xc675ad8a,0xf5f66923,0x79859a6c},
/*131*/{0x8f759b86,0x9a8669e2,0x79f6e86c,0x8a7583f5},
/*132*/{0xf783699c,0x79819b6c,0x9975a0d2,0xf5f669b7},
/*133*/{0x793ad66c,0x8b75e79a,0xb0cb6998,0x79969a6c},
/*134*/{0x9775eaed,0xf29c69a9,0x79bb886c,0x9d758284},
/*135*/{0xfd9669f3,0x79f9ea6c,0x8b75ac82,0xb79469b8},
/*136*/{0x799e816c,0x9a75b3fa,0xf5f6690c,0x79b0876c},
/*137*/{0x9875ff82,0x929d69a2,0x798ff66c,0xfa75e587},
/*138*/{0x00a469e0,0x7912bb6c,0xf275a680,0xb794690f},
/*139*/{0x799e816c,0xe9758d9c,0xa8ea6920,0x79f9ea6c},
/*140*/{0xec7530a7,0xf783698f,0x79ce916c,0x9175a78d},
/*141*/{0x89856987,0x7994fe6c,0x9375eaed,0x8d87698b},
/*142*/{0x79288b6c,0xc7758284,0xe0ed69a5,0x79eefb6c},
/*143*/{0x9d75a0d2,0xf5f669f3,0x799ff06c,0xe875ec84},
/*144*/{0xf89d6987,0x799efd6c,0xd575eaed,0xb19d69d5},
/*145*/{0x79a1816c,0xfa7509bf,0x1f8c69e0,0x79a6fd6c},
/*146*/{0x9175f5f7,0xb49c69b8,0x79f6e86c,0x8f7587f6},
/*147*/{0xc0d96912,0x79bd816c,0xaa75bd84,0xe6f6690c},
/*148*/{0x79ccc56c,0xc775e5ef,0x9b9a69a5,0x7996fc6c},
/*149*/{0x9175e79a,0x19a669b8,0x79f9ea6c,0x8f75ee86},
/*150*/{0xad9d69e2,0x7915ba6c,0x9375e5ef,0x0783698b},
/*151*/{0x79ccc56c,0x9175a184,0x19a669b8,0x79f6e86c},
/*152*/{0x8f7587f6,0x1f8c6912,0x79a6fd6c,0x9175f5f7},
/*153*/{0xb49c69b8,0x79eaea6c,0x9275a882,0x8d8769b5},
/*154*/{0x79859a6c,0xe275baf8,0x868069f0,0x79f9ea6c},
/*155*/{0x9775a0d2,0x898669ad,0x79b8806c,0xe575e5ef},
/*156*/{0xb89b69fc,0x793ad66c,0xfa75a485,0xb89b69e0},
/*157*/{0x79859a6c,0xe27587f6,0xe6f669f0,0x799ff06c},
/*158*/{0x8b750f95,0xb49c6998,0x79bcd76c,0x9675a580},
/*159*/{0x8680698e,0x79f9ea6c,0xe6753dc8,0x8d876985},
/*160*/{0x7991f56c,0x9575ffa0,0x9b9a69a0,0x79eeea6c},
/*161*/{0x8b7592dc,0xb49c6998,0x79bcd76c,0x9675a580},
/*162*/{0x8680698e,0x79f9ea6c,0xe6758288,0x8d876985},
/*163*/{0x79cf9a6c,0x9575ffa0,0x9b9a69a0,0x79f6e86c},
/*164*/{0x8f75dc9e,0xe2e769e2,0x79df9b6c,0x8d75b28f},
/*165*/{0x9d8769f7,0x79f9ea6c,0x8f75e98e,0xe2e769e2},
/*166*/{0x79d18b6c,0x8d75b28f,0xfa9a69f7,0x79f6e86c},
/*167*/{0x8f75e2fe,0xe2e769e2,0x7996fc6c,0x8d75b28f},
/*168*/{0xfb9b69f7,0x79f6e86c,0x917582f8,0x868069b1},
/*169*/{0x79eefb6c,0x8f7580f3,0xc881698e,0x79f9ea6c},
/*170*/{0xe27583f5,0xa1e369a0,0x79bcd76c,0xf875f29e},
/*171*/{0xbd9369ef,0x799a806c,0xe975f2fe,0xf5f669c5},
/*172*/{0x79b18f6c,0xeb758b83,0x989669f7,0x79f9ea6c},
/*173*/{0x9975028c,0xbb8c6907,0x79eefb6c,0x9a7580f3},
/*174*/{0xf5f669b7,0x7998c66c,0xb5753ed6,0x36ca69fa},
/*175*/{0x79ba876c,0x9375eaed,0x0783698b,0x793ad66c},
/*176*/{0x8f75a7ec,0xb7f569e2,0x79f9ea6c,0x967526d3},
/*177*/{0xf78369bf,0x79a6866c,0x9375e5ef,0x0783698b},
/*178*/{0x79e4aa6c,0xf87599f7,0xf48669ef,0x79fb9f6c},
/*179*/{0x8b75f2fe,0xbd9369c6,0x7982836c,0xb275e5ef},
/*180*/{0xf783691f,0x79eefb6c,0x9f75c39e,0xb8b469a8},
/*181*/{0x79f6e86c,0xe07585c4,0xf7836989,0x7995fc6c},
/*182*/{0x8b75e5ef,0xa0866998,0x79a4f46c,0x8a75e79a},
/*183*/{0xf5f6699c,0x79898d6c,0xe075e39a,0xe59f6989},
/*184*/{0x79f9ea6c,0x9a758e86,0x99e069b7,0x793af56c},
/*185*/{0xe075eaed,0xf7836989,0x7993806c,0x9375a0d2},
/*186*/{0x868069f3,0x79f9ea6c,0x9a75eb82,0xa78569a2},
/*187*/{0x79fd846c,0x9075e5ef,0xe2e769a2,0x799cf66c},
/*188*/{0xf8751b9b,0xabe269ef,0x79fc876c,0xe675f2fe},
/*189*/{0x0b826985,0x79f9ea6c,0xd67583f5,0xb0cb69c6},
/*190*/{0x79fb9f6c,0xf8759484,0xfc8169ef,0x79b7906c},
/*191*/{0xef75bd84,0xbf9c69e0,0x79fdfd6c,0xf375e5ef},
/*192*/{0xbb8c693d,0x79a1816c,0x9275fe82,0xd6b56988},
/*193*/{0x79f9ea6c,0x8075f9cf,0xbd9369ae,0x79a1816c},
/*194*/{0xfa7584f3,0x36c969e0,0x79b7906c,0xe875bd84},
/*195*/{0xf78369b5,0x79f7c56c,0xfa75c5ad,0xb29f69e0},
/*196*/{0x79b7906c,0x9175ad8a,0xbacd69b8,0x79f9ea6c},
/*197*/{0x8075aed9,0xbd9369ae,0x79a1816c,0xfa7581f5},
/*198*/{0xdfc969e0,0x79b7906c,0x9175ad8a,0xb8c869b8},
/*199*/{0x79f6e86c,0x9d75a286,0xb8c86990,0x79819b6c},
/*200*/{0xb57587ff,0xf7836923,0x79b9996c,0xfa75ac8b},
/*201*/{0xb29f69f3,0x79898d6c,0x8b75fbfc,0x9aeb6998},
/*202*/{0x7986cd6c,0x8975e79a,0xa39c69a0,0x79eaea6c},
/*203*/{0xed75eafd,0x99e06987,0x798a9c6c,0x8a75f2fe},
/*204*/{0xf68669b7,0x79f9ea6c,0x9775bbfb,0xe2e769e5},
/*205*/{0x79fa9a6c,0xe075e5ef,0x85916986,0x7983f86c},
/*206*/{0x8975f2fe,0x90e6698e,0x79f6e86c,0x95758b9a},
/*207*/{0x16cd69df,0x79f9ea6c,0xb5759186,0x0fef6923},
/*208*/{0x79f9ea6c,0x97752f84,0x12d469a3,0x79a29e6c},
/*209*/{0x9775e5ef,0x8d8769ad,0x790b9a6c,0xd675ad8a},
/*210*/{0xfaf46981,0x79a7fe6c,0x8575a682,0xe68969f3},
/*211*/{0x79f9ea6c,0xc375bef4,0xbd93693b,0x79acf16c},
/*212*/{0xc375eaed,0xa0ed693b,0x79ee9d6c,0xfa7526a0},
/*213*/{0x36b969e0,0x79ee9d6c,0x9a7584c3,0xfaf469b0},
/*214*/{0x7998c66c,0x8d75b58f,0xfae669f7,0x79f9ea6c},
/*215*/{0xea75e39a,0xe28169ef,0x79a28a6c,0xeb75e5ef},
/*216*/{0xf38369f7,0x79f6fa6c,0x9475e5ef,0xae8269b1},
/*217*/{0x79f9ea6c,0xf8759e9c,0xf38369ef,0x79f6fa6c},
/*218*/{0xfa759484,0x849d69e0,0x799e8d6c,0xfa75e2f5},
/*219*/{0xf2ec69e0,0x799e8d6c,0xfa75bc81,0xac9869e0},
/*220*/{0x799e8d6c,0xfa75a580,0xb59969e0,0x799e8d6c},
/*221*/{0xfa7589f9,0x99e069e0,0x799e8d6c,0xf8758af2},
/*222*/{0x04ce69ef,0x79b8806c,0xed75f2fe,0xfaf469b5},
/*223*/{0x79fe856c,0xfa758685,0x9b9a69e0,0x79ff9f6c},
/*224*/{0x8f75999f,0xfaf469e2,0x79b18f6c,0xfa7587f8},
/*225*/{0x118669e0,0x79adff6c,0x8f753c9b,0xfaf469e2},
/*226*/{0x79b18f6c,0xfa7587f8,0xee9b69e0,0x79fb9f6c},
/*227*/{0x9f75eaed,0x97e169a8,0x79f9ea6c,0x8f75a382},
/*228*/{0xfaf469e2,0x7990996c,0x8e75f2fe,0x887569af},
/*229*/{0x79f9ea85,0xeb75999f,0x9c8569f7,0x799c986c},
/*230*/{0xf375eaed,0xbb8c693d,0x79b18f6c,0x88750f95},
/*231*/{0xfaf46998,0x79f3f06c,0x937580ff,0xb59e698e},
/*232*/{0x79f9ea6c,0x897590fc,0x8de86999,0x790b9f6c},
/*233*/{0x8e759b82,0xe29d6998,0x79f6e86c,0xec75baf8},
/*234*/{0x0cda698c,0x79f9ea6c,0x9175999f,0x21946986},
/*235*/{0x79eeea6c,0xe2758e9a,0xafe66997,0x79f9ea6c},
/*236*/{0xed75999f,0x97d769bf,0x79eeea6c,0x8a75b08d},
/*237*/{0xe2e76904,0x7998f36c,0x8a75e5ef,0xfb85699c},
/*238*/{0x799dfd6c,0xde75f2ef,0x8be969b4,0x79bdf26c},
/*239*/{0xfa7530cd,0x898669e0,0x79879f6c,0xf875f5ff},
/*240*/{0x07e869ef,0x79288b6c,0x8f75c0db,0xf5f6698b},
/*241*/{0x79a48a6c,0xe975b28c,0xefe46995,0x79eeea6c},
/*242*/{0x917517f1,0xd0c26986,0x792d886c,0x9a75e5ef},
/*243*/{0x94da69bd,0x79f7996c,0xfa7581f8,0x07e869f7},
/*244*/{0x799c8e6c,0x9975c0db,0xf5f669b7,0x791cf86c},
/*245*/{0xc6751595,0x89866923,0x79f6e86c,0x917582f8},
/*246*/{0x868069b1,0x79f9ea6c,0xc7759d9e,0x9d9b69a5},
/*247*/{0x79eefb6c,0xfa75aa9b,0x93ec69e0,0x79bb886c},
/*248*/{0x9275999f,0x80d969b5,0x79f9ea6c,0x9275c39e},
/*249*/{0xfee46994,0x79b6d16c,0x9075e5ef,0xe49e6995},
/*250*/{0x7984f06c,0xf875b29f,0x07e869ef,0x7902c56c},
/*251*/{0xc87526d3,0xfaf46922,0x79fd876c,0x9c75e79a},
/*252*/{0xfbe5692f,0x79f9ea6c,0x9175a983,0x8edb69e9},
/*253*/{0x799bf36c,0x9075f6ef,0xf783698a,0x79368c6c},
/*254*/{0xfa75f3dc,0xb99a69e0,0x79f0816c,0x917587f6},
/*255*/{0xe6f669b8,0x799e816c,0x9275e79a,0xee9b69b5}};
#endif

void SIP_TKPT_Secret(const char* tkpt_packet, int len,char SCNo, char Factor, unsigned char *result)
{
	int i;
	int temp=0;
	unsigned char *key=(unsigned  char*)&Secret_Data[SCNo];
	result[0]=SCNo;
	result[1]=Factor;
	for ( i=2;i<len;i++)
	{
		result[i]= tkpt_packet[i]^key[temp]^Factor;
		temp++;
		if ( temp>=16)
			temp=0;
	}
}

int SIP_TKPT_DeSecret(const char* tkpt_packet, const int length, unsigned char *result)
{
	int i;
	int temp=0;
	unsigned char *key;
	unsigned short len;
	unsigned char SCNo,Factor;
	SCNo=tkpt_packet[0];
	Factor=tkpt_packet[1];
	key=(unsigned  char*)&Secret_Data[SCNo];
	result[0]=SCNo;
	result[1]=Factor;

	if(length<=4)
		return 0;

	for ( i=2;i<length;i++)
	{
		result[i]= tkpt_packet[i]^key[temp]^Factor;
		temp++;
		if ( temp>=16)
			temp=0;
	}

	len=(unsigned char)result[2];
	len<<=8;
	len +=(unsigned char)result[3];
	return (len == length);
}

int sip_encrypt(const char *sip_packet, unsigned char *result)
{
	char buf[4096];

	char scno = 2;
	char factor = '3';
	int packetLength=0;

	packetLength = strlen(sip_packet)+4;
	buf[0]=scno;
	buf[1]=factor;
    buf[2] = (unsigned char)(packetLength >> 8);
    buf[3] = (unsigned char)packetLength;
	memcpy(&buf[4],sip_packet,packetLength-4);
	SIP_TKPT_Secret(buf, packetLength, scno, factor, result);
	return packetLength;
}

int sip_decrypt(const char *encryptstr, const int packetLength, char *decryptstr)
{
	unsigned char buf[4096];
	if(SIP_TKPT_DeSecret(encryptstr, packetLength, buf))
	{
		memcpy(decryptstr, buf+4, packetLength-4);
		return 1;
	}
	return 0;
}

int check_sippacket(const char *sipcryptpacket, const int packetLength)
{
	unsigned short len;
	int i;
	int temp=0;
	unsigned char SCNo, Factor;
	char result[4];
	unsigned char *key;
	if(packetLength<=4)
		return 0;		
	SCNo=sipcryptpacket[0];
	Factor=sipcryptpacket[1];
	key=(unsigned  char*)&Secret_Data[SCNo];
	result[0]=SCNo;
	result[1]=Factor;
	for(i=2; i<4; i++)
	{
		result[i]= sipcryptpacket[i]^key[temp]^Factor;
		temp++;
		if( temp>=16)
			temp=0;
	}
	len=(unsigned char)result[2];
	len<<=8;
	len +=(unsigned char)result[3];
	if(len == packetLength)
		return 1;
	return 0;
}
/*
void encMsg(UINT8* pMsg,UINT16 msgLen)
{
 UINT8* pKey;
 UINT16 keyLen;
 UINT16 i,j;
 
 pKey = "1960b33098979b7e674c3b10b50d9a7c";//get key  keyLen = strlen((const char*)pKey);
 if(!keyLen)
 {
  return;
 }
 j=keyLen;
 for(i=msgLen; i>0; i--)
 {
  pMsg[i-1] ^= pKey[j-1];
  j--;
  if(!j)
  {
   j = keyLen;
  }
 }
}
*/
//encrypt message & decrypt message
void walkersunEncMsg(INT8U* pMsg, INT16U msgLen, unsigned char *result)	// walkersun
{
	INT8U* pKey;
	INT16U keyLen;
	INT16U i, j;
 
	pKey = (INT8U*)"1960b33098979b7e674c3b10b50d9a7c"; //get key
	keyLen = strlen((const char*)pKey);
	
	if(!keyLen)
		return;
		
	memcpy(result, pMsg, msgLen);
		
	j=keyLen;
	for(i=msgLen; i>0; i--)
	{
//		result[i-1] = pMsg[i-1] ^ pKey[j-1];
		result[i-1] ^= pKey[j-1];
		j--;
		if(!j)
			j = keyLen;
	}
}

int walkersun_sip_encrypt(const char *sip_packet, unsigned char *result)
{
	int SIP_Packet_Len;
	SIP_Packet_Len=strlen(sip_packet);
	walkersunEncMsg((INT8U*) sip_packet, strlen(sip_packet), result);
	return SIP_Packet_Len;
}

int walkersun_sip_decrypt(const char *encryptstr, const int packetLength, char *decryptstr)
{
	walkersunEncMsg((INT8U*) encryptstr, packetLength, (unsigned char*)decryptstr);
	return packetLength;
}

int walkersun_check_sippacket(const char *sipcryptpacket, const int packetLength)
{	//Jeffreys Modify
	INT8U* pKey;
	INT16U keyLen;
	INT16U i, j, Check_Len;
	char result[50];
 
	pKey = (INT8U*)"1960b33098979b7e674c3b10b50d9a7c"; //get key
	keyLen = strlen((const char*)pKey);

	if(!keyLen)	return -1;
		
	if ((Check_Len=packetLength%keyLen)<10)	{ j=10-Check_Len; Check_Len=10;}
	else j=keyLen;
	for(i=Check_Len; i>0; i--)
	{
		result[i-1] = sipcryptpacket[i-1] ^ pKey[j-1];
		j--;
		if(!j)	j = keyLen; 
	}
	result[Check_Len]=0;

	if			(!strncmp(result,"SIP/2.0",7))		return 1;
	else if		(!strncmp(result,"REGISTER",8))		return 1;
	else if		(!strncmp(result,"INVITE",6))		return 1;
	else if		(!strncmp(result,"ACK",3))			return 1;
	else if		(!strncmp(result,"BYE",3))			return 1;
	else if		(!strncmp(result,"CANCEL",6))		return 1;
	else if		(!strncmp(result,"REFER",5))		return 1;
	else if		(!strncmp(result,"NOTIFY",6))		return 1;
	else if		(!strncmp(result,"OPTIONS",7))		return 1;
	else if		(!strncmp(result,"QUERY",5))		return 1;
	else if		(!strncmp(result,"SUBSCRIBE",9))	return 1;
	else if		(!strncmp(result,"MESSAGE",7))		return 1;
	else if		(!strncmp(result,"PUBLISH",7))		return 1;
	else if		(!strncmp(result,"UPDATE",6))		return 1;
	else if		(!strncmp(result,"PRACK",5))		return 1;
	else	return 0;
}

SIP_ENCRYPT_TYPE getSipCryptType(void)
{
	return sipEncryptFunctionSelect;
}

SIP_ENCRYPT_FUN getSipCryptFun(void)
{
	if(sipEncryptFunctionSelect==SIP_ENCRYPT_AVS)
		return sip_encrypt;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_WALKSUN)
		return walkersun_sip_encrypt;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_SEAWOLF)
		return SeaWolf_sip_encrypt;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_SEAWOLF_N2C)
		return SeaWolf_sip_encrypt_N2C;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_GX_XOR)
		return GxXor_sip_encrypt;
	else
		return NULL;
}

SIP_DECRYPT_FUN getSipDeCryptFun(void)
{
	if(sipEncryptFunctionSelect==SIP_ENCRYPT_AVS)
		return sip_decrypt;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_WALKSUN)
		return walkersun_sip_decrypt;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_SEAWOLF)
		return SeaWolf_sip_decrypt;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_SEAWOLF_N2C)
		return SeaWolf_sip_decrypt_N2C;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_GX_XOR)
		return GxXor_sip_decrypt;
	else
		return NULL;
}

SIP_CHECK_SIP_ENCRYPT_FUN getSipCryptCheckFun(void)
{
	if(sipEncryptFunctionSelect==SIP_ENCRYPT_AVS)
		return check_sippacket;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_WALKSUN)
		return walkersun_check_sippacket;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_SEAWOLF)
		return SeaWolf_check_sippacket;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_SEAWOLF_N2C)
		return SeaWolf_check_sippacket;
	else if(sipEncryptFunctionSelect==SIP_ENCRYPT_GX_XOR)
		return GxXor_check_sippacket;
	else
		return NULL;
}

unsigned char krand(void)
{
	unsigned char key;
	key = (unsigned char)rand();
	return key&0x7F;
}

/*encrypt rtp header*/
void walkersun_rtp_encrtphdr(unsigned char* rtppack, t_walkersun_rtp_encpacket* epack)
{
	unsigned char key;
	unsigned char tmp;
  
	key = krand();
	epack->key = key;
	epack->packet = rtppack;
	tmp = (epack->key >> 7);
	key <<= 1;
	tmp |= key;
	*(epack->packet) ^= tmp;
  
	tmp = (epack->key >> 6);
	key <<= 1;
	tmp |= key;
	*(epack->packet+1) ^= tmp;
  
	tmp = (epack->key >> 5);
	key <<= 1;
	tmp |= key;
	*(epack->packet+2) ^= tmp;
  
	tmp = (epack->key >> 4);
	key <<= 1;
	tmp |= key;
	*(epack->packet+3) ^= tmp;
}

/*decrypt RTP header. valid the key before invoking*/
unsigned char* walkersun_rtp_decrtphdr(t_walkersun_rtp_encpacket* epack)
{
	unsigned char key;
	unsigned char tmp;
  
	key = epack->key;

	tmp = (epack->key >> 7);
	key <<= 1;
	tmp |= key;
	*(epack->packet) ^= tmp;
  
	tmp = (epack->key >> 6);
	key <<= 1;
	tmp |= key;
	*(epack->packet+1) ^= tmp;
  
	tmp = (epack->key >> 5);
	key <<= 1;
	tmp |= key;
	*(epack->packet+2) ^= tmp;
  
	tmp = (epack->key >> 4);
	key <<= 1;
	tmp |= key;
	*(epack->packet+3) ^= tmp;
  
	return epack->packet;
}

int WallkersunRtpEncrypt(const unsigned char* rtp_pkt, const int nPktLen, unsigned char* result)
{
	t_walkersun_rtp_encpacket encpacket;
	memcpy(result+1, rtp_pkt, nPktLen);
	walkersun_rtp_encrtphdr(result+1, &encpacket);
	*result = encpacket.key;
	return nPktLen+1;
}

int WallkersunRtpDecrypt(const unsigned char* encrypt_pkt, const int nPktLen, unsigned char* decrypt)
{
	t_walkersun_rtp_encpacket encpacket;
	encpacket.key = *encrypt_pkt;
	memcpy(decrypt, encrypt_pkt+1, nPktLen-1);
	encpacket.packet = decrypt;
	walkersun_rtp_decrtphdr(&encpacket);
	return nPktLen-1;
}

RTP_ENCRYPT_TYPE getRtpCryptType(void)
{
	return rtpEncryptFunctionSelect;
}

// follow function will be called by CDRtpUdpStack constructor
RTP_ENCRYPT_FUN getRtpCryptFun(void)
{
	if(rtpEncryptFunctionSelect==RTP_ENCRYPT_WALKSUN)
		return WallkersunRtpEncrypt;
	if(rtpEncryptFunctionSelect==RTP_ENCRYPT_SEAWOLF)
		return SeaWolf_rtp_encrypt;
	if(rtpEncryptFunctionSelect==RTP_ENCRYPT_SEAWOLF_N2C)
		return SeaWolf_rtp_encrypt_N2C;
	if(rtpEncryptFunctionSelect==RTP_ENCRYPT_GX_XOR)
		return GxXor_rtp_encrypt;
	else
		return NULL;
}

RTP_DECRYPT_FUN getRtpDeCryptFun(void)
{
	if(rtpEncryptFunctionSelect==RTP_ENCRYPT_WALKSUN)
		return WallkersunRtpDecrypt;
	if(rtpEncryptFunctionSelect==RTP_ENCRYPT_SEAWOLF)
		return SeaWolf_rtp_decrypt;
	if(rtpEncryptFunctionSelect==RTP_ENCRYPT_SEAWOLF_N2C)
		return SeaWolf_rtp_decrypt_N2C;
	if(rtpEncryptFunctionSelect==RTP_ENCRYPT_GX_XOR)
		return GxXor_rtp_decrypt;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//
#define SEA_WOLF_EXTRA_SIP_CRYPT_HEAD_LEN 18
#define SEA_WOLF_EXTRA_RTP_CRYPT_HEAD_LEN 8
int SeaWolf_sip_encrypt(const char *sip_packet, unsigned char *result)
{
	int i;
	int packLen = strlen(sip_packet);
	int encLen=packLen+SEA_WOLF_EXTRA_SIP_CRYPT_HEAD_LEN;
	result[6]=0xff;
	result[7]=0xff;
	for(i=0; i<packLen; i++)
		result[i+SEA_WOLF_EXTRA_SIP_CRYPT_HEAD_LEN]=sip_packet[i]^0xff;
	return encLen;
}

int SeaWolf_sip_decrypt(const char *encryptstr, const int packetLength, char *decryptstr)
{
	int decodeLen = 0;
	int i;
	if(packetLength<=SEA_WOLF_EXTRA_SIP_CRYPT_HEAD_LEN) 
	{
		return decodeLen;
	}

	decodeLen = packetLength - SEA_WOLF_EXTRA_SIP_CRYPT_HEAD_LEN;
	for(i=0; i<decodeLen; i++)
		decryptstr[i] = encryptstr[SEA_WOLF_EXTRA_SIP_CRYPT_HEAD_LEN+i]^0xff;
	return decodeLen;
}

int SeaWolf_rtp_encrypt(const unsigned char *sip_packet, const int packetlen, unsigned char *result)
{
	int i;
	int encLen=packetlen+SEA_WOLF_EXTRA_RTP_CRYPT_HEAD_LEN;
	result[6]=0xff;
	result[7]=0xff;
	for(i=0; i<packetlen; i++)
		result[i+SEA_WOLF_EXTRA_SIP_CRYPT_HEAD_LEN]=sip_packet[i]^0xff;
	return encLen;
}

int SeaWolf_rtp_decrypt(const unsigned char *encryptstr, const int packetLength, unsigned char *decryptstr)
{
	int i;
	int decodeLen = 0;
	if(packetLength<=SEA_WOLF_EXTRA_RTP_CRYPT_HEAD_LEN) 
	{
		return decodeLen;
	}

	decodeLen = packetLength - SEA_WOLF_EXTRA_SIP_CRYPT_HEAD_LEN;
	for(i=0; i<decodeLen; i++)
		decryptstr[i] = encryptstr[SEA_WOLF_EXTRA_SIP_CRYPT_HEAD_LEN+i]^0xff;
	return decodeLen;
}


///////////////////////////////////////////////////////////////////////////////////////////
//
//
N2C_Sec_ErrorCode  N2C_Map_Encode_Algorithm(const char* msg_buffer,
											unsigned short len,
											char* encode_buffer,
											unsigned int* actuallen,
											const char* EncodeMapTable,
											int MapSize,
											unsigned short EncryptMethod)
{
	unsigned short* N2C_ptr;
	char* ptr;
	int i;
	const unsigned char* src;
	char* encode_ptr ;
	unsigned short tmpEncryptMethod;
	int encodelen;

	//printf("in N2C_Map_Encode_Algorithm\n\n");			 

 	if((0>=len)||(len+sizeof(N2C_MAPHEADER)>=*actuallen))
 	{
		return Encode_MsgLen_Invalid;
 	}

	if((NULL == msg_buffer)||(NULL == encode_buffer))
	{
		return Encode_MsgBuf_Invalid;
	}

	if(NULL == EncodeMapTable)
	{
		return Encode_MapTable_Invalid;
	}

	if(0>= MapSize)
	{
		return Encode_MapSize_Invalid;
	}
		
	encode_ptr = encode_buffer;
	encodelen = *actuallen;
	//tmpEncryptMethod = EncryptMethod;
	tmpEncryptMethod = 0x0001;
	*actuallen = len+sizeof(N2C_MAPHEADER);

	// add UDP header
	N2C_ptr = (unsigned short*)encode_ptr;
	N2C_ptr[0] = htons(tmpEncryptMethod);       // checksun
	N2C_ptr[1] = htons(len);                 // len

	ptr = encode_ptr+sizeof(N2C_MAPHEADER);
	src = (unsigned char *)msg_buffer;

	//printf("begin to encode  MapSize = %d,len = %d\n\n",MapSize,len);			 
	
	if(255<=MapSize)
	{
		for(i=0;i<len;i++)
		{
			*ptr = *(EncodeMapTable+*src);
			//printf("prt = %d, str = %d\n\n" ,*ptr,*src);			 
			ptr++;
			src++;
		}
		
	}
	else
	{
		for(i = 0;i<len;i++)
		{
			if(*src>MapSize)
			{
				return Encode_MapSize_TooSmall;
			}
			
			*ptr = *(EncodeMapTable+*src);
			ptr++;
			src++;
		}
	}
	
	return Sec_Success;
}

N2C_Sec_ErrorCode N2C_Map_Decode_Algorithm(const char *msg_ptr,
										   unsigned short len,
										   char *decode_buffer,
										   unsigned int *actuallen,
										   const char *DecodeMapTable,
										   int MapSize,
										   unsigned short EncryptMethod)
{
	const N2C_MAPHEADER* MapHead;
	char* output;
	const unsigned char* src;
	const char* output_msg_ptr;
	unsigned short tmpEncryptMethod;

	if((8>=len)||(len-sizeof(N2C_MAPHEADER)>=*actuallen))
 	{
		return Decode_MsgLen_Invalid;
 	}
	
	if((NULL == msg_ptr)||(NULL == decode_buffer))
	{
		return Decode_MsgBuf_Invalid;

	}

	if(NULL == DecodeMapTable)
	{
		return Decode_MapTable_Invalid;

	}

	if(0>= MapSize)
	{
		return Decode_MapSize_Invalid;
	}
	
	output_msg_ptr = decode_buffer;
	//tmpEncryptMethod = EncryptMethod;
	tmpEncryptMethod = 0x0001;
	src = (const unsigned char* )msg_ptr;
	MapHead= (const N2C_MAPHEADER*)src;

	if(tmpEncryptMethod!=ntohs(MapHead->EncryptMethod ))
	{
		return Decode_EncrypMethod_Invalid;
	}
	
	if((len-sizeof(N2C_MAPHEADER))!=ntohs(MapHead->Length) )
	{
		return Decode_MsgBodyLength_Invalid;
	}

	*actuallen= len-sizeof(N2C_MAPHEADER);

	src = (const unsigned char* )(msg_ptr  + sizeof(N2C_MAPHEADER));
	output = decode_buffer;
 
	if(255<=MapSize)
	{
		unsigned int i;
		for(i = 0;i<*actuallen;i++)
		{
			*output = *(DecodeMapTable+*src);
			//printf("output = %d, str = %d\n\n" ,*output,*src);			 
			output++;
			src++;			
		}
	}
	else
	{
		unsigned int i;
		for(i = 0;i<*actuallen;i++)
		{
			if(*src>MapSize)
			{
				return Encode_MapSize_TooSmall;
			}

			*output = *(DecodeMapTable+*src);
			output++;
			src++;					
		}
	}
	return Sec_Success;
}

#define Max_Table_Size 256
#define Max_Buf_Len 2048
#define Bit_DeBit_Table	// for sip encrypt
//#define Rand_Table
#define Regular_Table	// for rtp encrypt
//#define Print_Table
char EncodeSipMapTable[512];
char DecodeSipMapTable[512];
char EncodeRtpMapTable[512];
char DecodeRtpMapTable[512];
int MapSize;
static unsigned char boSeaWolfMapInit = 0;


int InitSecurityMap(void)
{
	int tmpnum = 0;
	int i,j;
	char tmpchar[Max_Table_Size];

	MapSize = Max_Table_Size;
	memset(DecodeSipMapTable, 0, Max_Table_Size);
	memset(EncodeSipMapTable, 0, Max_Table_Size);
	memset(DecodeRtpMapTable, 0, Max_Table_Size);
	memset(EncodeRtpMapTable, 0, Max_Table_Size);

#ifdef Bit_DeBit_Table	
	for(i = 0;i<MapSize;i++)
	{
		EncodeSipMapTable[i] = MapSize - i-1;
	}

	memcpy(DecodeSipMapTable, EncodeSipMapTable,256);
#endif

#ifdef Rand_Table
	char tmpchar[256];
	memset(tmpchar,0,256);
	int tmpnum = 0;
	for(i=0; i<MapSize; i++)
	{
		tmpchar[i] = i;
	}

	for(unsigned int j=0; j<MapSize; j++)
	{
		unsigned int num=0;
		num = rand()%(256-j);
		EncodeMapTable[j]= tmpchar[num];
		DecodeMapTable[(unsigned int)tmpchar[num]]=j;
		if(0==tmpnum%6)
		{
			printf("\n");
		}
		printf(" Encode[%d]= %d      ",j,EncodeMapTable[j]);
		tmpnum++;
		if(num==(255-j))
			tmpchar[num]=-1;
		else
		{
			for(;num<(255-j);num++)
			{
				tmpchar[num]=tmpchar[num+1];
			}
			tmpchar[255-j] = -1;
		}
					
	}
#endif
	
#ifdef Regular_Table
	memset(tmpchar,0,Max_Table_Size);
	for(i=0; i<MapSize; i++)
	{
		tmpchar[i] = i;
	}

	for(j=0; j<MapSize; j++)
	{
		int num=0;
		num = (j*11)%(Max_Table_Size-j);
		EncodeRtpMapTable[j]= tmpchar[num];
		DecodeRtpMapTable[(unsigned char)tmpchar[num]]=j;
		if(num==(Max_Table_Size-1-j))
			tmpchar[num]=-1;
		else
		{
			for(;num<(Max_Table_Size -1-j);num++)
			{
				tmpchar[num]=tmpchar[num+1];
			}
			tmpchar[Max_Table_Size -1-j] = -1;
		}
					
	}
#endif

#ifdef Print_Table
	//printf the tabale
	printf("the EncodeMapTable is: \n\n");
	tmpnum=0;
	for(i = 0 ;i<Max_Table_Size;i++)
	{
		if(0==tmpnum%50)
		{
			printf("\n");
		}	
		if((EncodeMapTable[i]<16)&&(EncodeMapTable[i]>=0))
		{
			printf("0%x",(unsigned char)EncodeMapTable[i]);

		}
		else
		{
			printf("%x",(unsigned char)EncodeMapTable[i]);
		}
		tmpnum++;
		
	}

	printf("\n\nthe DecodeMapTable is: \n\n");
	tmpnum=0;
	for(i = 0 ;i<Max_Table_Size;i++)
	{
		if(0==tmpnum%50)
		{
			printf("\n");
		}		
		if((DecodeMapTable[i]<16)&&(DecodeMapTable[i]>=0))
		{
			printf("0%x",(unsigned char)DecodeMapTable[i]);

		}
		else
		{
			printf("%x",(unsigned char)DecodeMapTable[i]);		
		}
		tmpnum++;
		
	}
#endif
	boSeaWolfMapInit = 1;
	return 1;
}

int SeaWolf_sip_encrypt_N2C(const char *sip_packet, unsigned char *result)
{
	unsigned short SipMsgLen = strlen(sip_packet);
	unsigned int EncodeDataLength = Max_Buf_Len;
	N2C_Sec_ErrorCode ret = Sec_Success;

	if(!boSeaWolfMapInit)
		InitSecurityMap();

	//encode the data
	ret = N2C_Map_Encode_Algorithm(sip_packet, SipMsgLen, (char*)result, &EncodeDataLength, 
									(const char *)EncodeSipMapTable, MapSize,0);
	if(Sec_Success!=ret)
	{
//		TRACE("Error When Encode, The Error Code is:%d\n",ret);
		return 0;
	}

	return EncodeDataLength;
}

int SeaWolf_sip_decrypt_N2C(const char *encryptstr, const int packetLength, char *decryptstr)
{
	unsigned int decodeLength;
	N2C_Sec_ErrorCode ret = Sec_Success;

	if(!boSeaWolfMapInit)
		InitSecurityMap();

	ret = N2C_Map_Decode_Algorithm(encryptstr, (unsigned short)packetLength, decryptstr, 
									&decodeLength,(const char *)DecodeSipMapTable, MapSize,0);
	if(Sec_Success!=ret)
	{
//		TRACE("Error When Decode,The Error Code is:%d\n",ret);
		return 0;
	}

	return decodeLength;
}

int SeaWolf_rtp_encrypt_N2C(const unsigned char *rtp_packet, const int packetlen, unsigned char *result)
{
	unsigned int EncodeDataLength = Max_Buf_Len;
	N2C_Sec_ErrorCode ret = Sec_Success;

	if(!boSeaWolfMapInit)
		InitSecurityMap();

	//encode the data
	ret = N2C_Map_Encode_Algorithm((const char *)rtp_packet, packetlen, (char*)result, &EncodeDataLength, 
									(const char *)EncodeRtpMapTable, MapSize,0);
	if(Sec_Success!=ret)
	{
//		TRACE("Error When Encode, The Error Code is:%d\n",ret);
		return 0;
	}

	return EncodeDataLength;
}

int SeaWolf_rtp_decrypt_N2C(const unsigned char *encryptstr, const int packetLength, unsigned char *decryptstr)
{
	unsigned int decodeLength;
	N2C_Sec_ErrorCode ret = Sec_Success;

	if(!boSeaWolfMapInit)
		InitSecurityMap();

	ret = N2C_Map_Decode_Algorithm((const char *)encryptstr, (unsigned short)packetLength, (char *)decryptstr, 
									&decodeLength,(const char *)DecodeRtpMapTable, MapSize,0);
	if(Sec_Success!=ret)
	{
//		TRACE("Error When Decode,The Error Code is:%d\n",ret);
		return 0;
	}

	return decodeLength;
}

int SeaWolf_check_sippacket(const char *sipcryptpacket, const int packetLength)
{
	return 1;
}

int encrypt_xor(char *input, char *output, int len)
{
	int i;
	if(output == NULL || input == NULL)return -1;
	if((len % 2)==0 )
	{
		for(i=0 ; i < len ; i+=2)
		{
			output[i] = input[i] ^ 0x47;
			output[i+1] = input[i+1] ^0x53;
		}
	}
	else
	{
		for(i=0 ; i < len - 1 ; i+=2)
		{
			//(*(short *)(&output[i])) =  (*(short *)(&input[i])) ^ (0x4753);
			output[i] = input[i] ^ 0x47;
			output[i+1] = input[i+1] ^0x53;
		} 
		output[len -1] = input[len -1 ] ^ (0x53);
	}
	return 0;
}

int GxXor_sip_encrypt(const char *sip_packet, unsigned char *result)
{
	int length = strlen(sip_packet);
	encrypt_xor((char*)sip_packet, (char*)result, length);
	return length;
}

int GxXor_sip_decrypt(const char *encryptstr, const int packetLength, char *decryptstr)
{
	encrypt_xor((char*)encryptstr, (char*)decryptstr, packetLength);
	return packetLength;
}

int GxXor_rtp_encrypt(const unsigned char *rtp_packet, const int packetlen, unsigned char *result)
{
	encrypt_xor((char*)rtp_packet, (char*)result, packetlen);
	return packetlen;
}

int GxXor_rtp_decrypt(const unsigned char *encryptstr, const int packetLength, unsigned char *decryptstr)
{
	encrypt_xor((char*)encryptstr, (char*)decryptstr, packetLength);
	return packetLength;
}

int GxXor_check_sippacket(const char *sipcryptpacket, const int packetLength)
{
	return 1;
}


