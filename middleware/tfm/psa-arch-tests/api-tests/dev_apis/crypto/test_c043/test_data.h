/** @file
 * Copyright (c) 2019-2020, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "val_crypto.h"

typedef struct {
    char                    test_desc[75];
    psa_key_type_t          key_type;
    uint8_t                 key_data[91];
    uint32_t                key_length;
    psa_key_usage_t         usage;
    psa_algorithm_t         key_alg;
    uint8_t                 peer_key[120];
    size_t                  peer_key_length;
    uint8_t                 expected_output[48];
    size_t                  expected_output_length;
    size_t                  output_size;
    psa_status_t            expected_status;
} test_data;


static const test_data check1[] = {
#ifdef ARCH_TEST_ECDH
#ifdef ARCH_TEST_ECC_CURVE_SECP256R1
{"Test psa_raw_key_agreement - ECDH SECP256R1\n",
 PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1),
{0xc8, 0x8f, 0x01, 0xf5, 0x10, 0xd9, 0xac, 0x3f, 0x70, 0xa2, 0x92, 0xda, 0xa2,
 0x31, 0x6d, 0xe5, 0x44, 0xe9, 0xaa, 0xb8, 0xaf, 0xe8, 0x40, 0x49, 0xc6, 0x2a,
 0x9c, 0x57, 0x86, 0x2d, 0x14, 0x33}, 32, PSA_KEY_USAGE_DERIVE, PSA_ALG_ECDH,
{0x04, 0xd1, 0x2d, 0xfb, 0x52, 0x89, 0xc8, 0xd4, 0xf8, 0x12, 0x08, 0xb7, 0x02,
 0x70, 0x39, 0x8c, 0x34, 0x22, 0x96, 0x97, 0x0a, 0x0b, 0xcc, 0xb7, 0x4c, 0x73,
 0x6f, 0xc7, 0x55, 0x44, 0x94, 0xbf, 0x63, 0x56, 0xfb, 0xf3, 0xca, 0x36, 0x6c,
 0xc2, 0x3e, 0x81, 0x57, 0x85, 0x4c, 0x13, 0xc5, 0x8d, 0x6a, 0xac, 0x23, 0xf0,
 0x46, 0xad, 0xa3, 0x0f, 0x83, 0x53, 0xe7, 0x4f, 0x33, 0x03, 0x98, 0x72, 0xab},
 65,
{0xd6, 0x84, 0x0f, 0x6b, 0x42, 0xf6, 0xed, 0xaf, 0xd1, 0x31, 0x16, 0xe0, 0xe1,
 0x25, 0x65, 0x20, 0x2f, 0xef, 0x8e, 0x9e, 0xce, 0x7d, 0xce, 0x03, 0x81, 0x24,
 0x64, 0xd0, 0x4b, 0x94, 0x42, 0xde}, 32, 32, PSA_SUCCESS
},

{"Test psa_raw_key_agreement - Small buffer size\n",
 PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1),
{0xc8, 0x8f, 0x01, 0xf5, 0x10, 0xd9, 0xac, 0x3f, 0x70, 0xa2, 0x92, 0xda, 0xa2,
 0x31, 0x6d, 0xe5, 0x44, 0xe9, 0xaa, 0xb8, 0xaf, 0xe8, 0x40, 0x49, 0xc6, 0x2a,
 0x9c, 0x57, 0x86, 0x2d, 0x14, 0x33}, 32, PSA_KEY_USAGE_DERIVE, PSA_ALG_ECDH,
{0x04, 0xd1, 0x2d, 0xfb, 0x52, 0x89, 0xc8, 0xd4, 0xf8, 0x12, 0x08, 0xb7, 0x02,
 0x70, 0x39, 0x8c, 0x34, 0x22, 0x96, 0x97, 0x0a, 0x0b, 0xcc, 0xb7, 0x4c, 0x73,
 0x6f, 0xc7, 0x55, 0x44, 0x94, 0xbf, 0x63, 0x56, 0xfb, 0xf3, 0xca, 0x36, 0x6c,
 0xc2, 0x3e, 0x81, 0x57, 0x85, 0x4c, 0x13, 0xc5, 0x8d, 0x6a, 0xac, 0x23, 0xf0,
 0x46, 0xad, 0xa3, 0x0f, 0x83, 0x53, 0xe7, 0x4f, 0x33, 0x03, 0x98, 0x72, 0xab},
 65,
{0xd6, 0x84, 0x0f, 0x6b, 0x42, 0xf6, 0xed, 0xaf, 0xd1, 0x31, 0x16, 0xe0, 0xe1,
 0x25, 0x65, 0x20, 0x2f, 0xef, 0x8e, 0x9e, 0xce, 0x7d, 0xce, 0x03, 0x81, 0x24,
 0x64, 0xd0, 0x4b, 0x94, 0x42, 0xde}, 32, 10, PSA_ERROR_INVALID_ARGUMENT
},

#endif

#ifdef ARCH_TEST_ECC_CURVE_SECP384R1
{"Test psa_raw_key_agreement - ECDH SECP384R1\n",
 PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1),
{0x09, 0x9f, 0x3c, 0x70, 0x34, 0xd4, 0xa2, 0xc6, 0x99, 0x88, 0x4d, 0x73, 0xa3,
 0x75, 0xa6, 0x7f, 0x76, 0x24, 0xef, 0x7c, 0x6b, 0x3c, 0x0f, 0x16, 0x06, 0x47,
 0xb6, 0x74, 0x14, 0xdc, 0xe6, 0x55, 0xe3, 0x5b, 0x53, 0x80, 0x41, 0xe6, 0x49,
 0xee, 0x3f, 0xae, 0xf8, 0x96, 0x78, 0x3a, 0xb1, 0x94}, 48, PSA_KEY_USAGE_DERIVE,
 PSA_ALG_ECDH,
{0x04, 0xe5, 0x58,
 0xdb, 0xef, 0x53, 0xee, 0xcd, 0xe3, 0xd3, 0xfc, 0xcf, 0xc1, 0xae, 0xa0, 0x8a,
 0x89, 0xa9, 0x87, 0x47, 0x5d, 0x12, 0xfd, 0x95, 0x0d, 0x83, 0xcf, 0xa4, 0x17,
 0x32, 0xbc, 0x50, 0x9d, 0x0d, 0x1a, 0xc4, 0x3a, 0x03, 0x36, 0xde, 0xf9, 0x6f,
 0xda, 0x41, 0xd0, 0x77, 0x4a, 0x35, 0x71, 0xdc, 0xfb, 0xec, 0x7a, 0xac, 0xf3,
 0x19, 0x64, 0x72, 0x16, 0x9e, 0x83, 0x84, 0x30, 0x36, 0x7f, 0x66, 0xee, 0xbe,
 0x3c, 0x6e, 0x70, 0xc4, 0x16, 0xdd, 0x5f, 0x0c, 0x68, 0x75, 0x9d, 0xd1, 0xff,
 0xf8, 0x3f, 0xa4, 0x01, 0x42, 0x20, 0x9d, 0xff, 0x5e, 0xaa, 0xd9, 0x6d, 0xb9,
 0xe6, 0x38, 0x6c}, 97,
{0x11, 0x18, 0x73, 0x31, 0xc2, 0x79, 0x96, 0x2d, 0x93, 0xd6, 0x04, 0x24, 0x3f,
 0xd5, 0x92, 0xcb, 0x9d, 0x0a, 0x92, 0x6f, 0x42, 0x2e, 0x47, 0x18, 0x75, 0x21,
 0x28, 0x7e, 0x71, 0x56, 0xc5, 0xc4, 0xd6, 0x03, 0x13, 0x55, 0x69, 0xb9, 0xe9,
 0xd0, 0x9c, 0xf5, 0xd4, 0xa2, 0x70, 0xf5, 0x97, 0x46}, 48, 48, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_ECC_CURVE_SECP256R1
{"Test psa_raw_key_agreement - Invalid usage\n",
 PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1),
{0xc8, 0x8f, 0x01, 0xf5, 0x10, 0xd9, 0xac, 0x3f, 0x70, 0xa2, 0x92, 0xda, 0xa2,
 0x31, 0x6d, 0xe5, 0x44, 0xe9, 0xaa, 0xb8, 0xaf, 0xe8, 0x40, 0x49, 0xc6, 0x2a,
 0x9c, 0x57, 0x86, 0x2d, 0x14, 0x33}, 32, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_ECDH,
{0x04, 0xd1, 0x2d, 0xfb, 0x52, 0x89, 0xc8, 0xd4, 0xf8, 0x12, 0x08, 0xb7, 0x02,
 0x70, 0x39, 0x8c, 0x34, 0x22, 0x96, 0x97, 0x0a, 0x0b, 0xcc, 0xb7, 0x4c, 0x73,
 0x6f, 0xc7, 0x55, 0x44, 0x94, 0xbf, 0x63, 0x56, 0xfb, 0xf3, 0xca, 0x36, 0x6c,
 0xc2, 0x3e, 0x81, 0x57, 0x85, 0x4c, 0x13, 0xc5, 0x8d, 0x6a, 0xac, 0x23, 0xf0,
 0x46, 0xad, 0xa3, 0x0f, 0x83, 0x53, 0xe7, 0x4f, 0x33, 0x03, 0x98, 0x72, 0xab},
 65,
{0xd6, 0x84, 0x0f, 0x6b, 0x42, 0xf6, 0xed, 0xaf, 0xd1, 0x31, 0x16, 0xe0, 0xe1,
 0x25, 0x65, 0x20, 0x2f, 0xef, 0x8e, 0x9e, 0xce, 0x7d, 0xce, 0x03, 0x81, 0x24,
 0x64, 0xd0, 0x4b, 0x94, 0x42, 0xde}, 32, 32, PSA_ERROR_NOT_PERMITTED
},

{"Test psa_raw_key_agreement - Unknown KDF\n",
 PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1),
{0xc8, 0x8f, 0x01, 0xf5, 0x10, 0xd9, 0xac, 0x3f, 0x70, 0xa2, 0x92, 0xda, 0xa2,
 0x31, 0x6d, 0xe5, 0x44, 0xe9, 0xaa, 0xb8, 0xaf, 0xe8, 0x40, 0x49, 0xc6, 0x2a,
 0x9c, 0x57, 0x86, 0x2d, 0x14, 0x33}, 32, PSA_KEY_USAGE_DERIVE,
 PSA_ALG_KEY_AGREEMENT(PSA_ALG_ECDH, PSA_HASH_ALG_INVALID),
{0x04, 0xd1, 0x2d, 0xfb, 0x52, 0x89, 0xc8, 0xd4, 0xf8, 0x12, 0x08, 0xb7, 0x02,
 0x70, 0x39, 0x8c, 0x34, 0x22, 0x96, 0x97, 0x0a, 0x0b, 0xcc, 0xb7, 0x4c, 0x73,
 0x6f, 0xc7, 0x55, 0x44, 0x94, 0xbf, 0x63, 0x56, 0xfb, 0xf3, 0xca, 0x36, 0x6c,
 0xc2, 0x3e, 0x81, 0x57, 0x85, 0x4c, 0x13, 0xc5, 0x8d, 0x6a, 0xac, 0x23, 0xf0,
 0x46, 0xad, 0xa3, 0x0f, 0x83, 0x53, 0xe7, 0x4f, 0x33, 0x03, 0x98, 0x72, 0xab},
 65,
{0xd6, 0x84, 0x0f, 0x6b, 0x42, 0xf6, 0xed, 0xaf, 0xd1, 0x31, 0x16, 0xe0, 0xe1,
 0x25, 0x65, 0x20, 0x2f, 0xef, 0x8e, 0x9e, 0xce, 0x7d, 0xce, 0x03, 0x81, 0x24,
 0x64, 0xd0, 0x4b, 0x94, 0x42, 0xde}, 32, 32, PSA_ERROR_INVALID_ARGUMENT
},
#endif
#endif

#ifdef ARCH_TEST_ECC_CURVE_SECP256R1
#ifdef ARCH_TEST_SHA256
#ifdef ARCH_TEST_HKDF
{"Test psa_raw_key_agreement - Not a key agreement alg\n",
 PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1),
{0xc8, 0x8f, 0x01, 0xf5, 0x10, 0xd9, 0xac, 0x3f, 0x70, 0xa2, 0x92, 0xda, 0xa2,
 0x31, 0x6d, 0xe5, 0x44, 0xe9, 0xaa, 0xb8, 0xaf, 0xe8, 0x40, 0x49, 0xc6, 0x2a,
 0x9c, 0x57, 0x86, 0x2d, 0x14, 0x33}, 32, PSA_KEY_USAGE_DERIVE, PSA_ALG_HKDF(PSA_ALG_SHA_256),
{0x04, 0xd1, 0x2d, 0xfb, 0x52, 0x89, 0xc8, 0xd4, 0xf8, 0x12, 0x08, 0xb7, 0x02,
 0x70, 0x39, 0x8c, 0x34, 0x22, 0x96, 0x97, 0x0a, 0x0b, 0xcc, 0xb7, 0x4c, 0x73,
 0x6f, 0xc7, 0x55, 0x44, 0x94, 0xbf, 0x63, 0x56, 0xfb, 0xf3, 0xca, 0x36, 0x6c,
 0xc2, 0x3e, 0x81, 0x57, 0x85, 0x4c, 0x13, 0xc5, 0x8d, 0x6a, 0xac, 0x23, 0xf0,
 0x46, 0xad, 0xa3, 0x0f, 0x83, 0x53, 0xe7, 0x4f, 0x33, 0x03, 0x98, 0x72, 0xab},
 65,
{0xd6, 0x84, 0x0f, 0x6b, 0x42, 0xf6, 0xed, 0xaf, 0xd1, 0x31, 0x16, 0xe0, 0xe1,
 0x25, 0x65, 0x20, 0x2f, 0xef, 0x8e, 0x9e, 0xce, 0x7d, 0xce, 0x03, 0x81, 0x24,
 0x64, 0xd0, 0x4b, 0x94, 0x42, 0xde}, 32, 32, PSA_ERROR_INVALID_ARGUMENT
},
#endif
#endif

#ifdef ARCH_TEST_ECDH
{"Test psa_raw_key_agreement - Public key on different curve\n",
 PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1),
{0xc8, 0x8f, 0x01, 0xf5, 0x10, 0xd9, 0xac, 0x3f, 0x70, 0xa2, 0x92, 0xda, 0xa2,
 0x31, 0x6d, 0xe5, 0x44, 0xe9, 0xaa, 0xb8, 0xaf, 0xe8, 0x40, 0x49, 0xc6, 0x2a,
 0x9c, 0x57, 0x86, 0x2d, 0x14, 0x33}, 32, PSA_KEY_USAGE_DERIVE, PSA_ALG_ECDH,
{0x04, 0xe5, 0x58,
 0xdb, 0xef, 0x53, 0xee, 0xcd, 0xe3, 0xd3, 0xfc, 0xcf, 0xc1, 0xae, 0xa0, 0x8a,
 0x89, 0xa9, 0x87, 0x47, 0x5d, 0x12, 0xfd, 0x95, 0x0d, 0x83, 0xcf, 0xa4, 0x17,
 0x32, 0xbc, 0x50, 0x9d, 0x0d, 0x1a, 0xc4, 0x3a, 0x03, 0x36, 0xde, 0xf9, 0x6f,
 0xda, 0x41, 0xd0, 0x77, 0x4a, 0x35, 0x71, 0xdc, 0xfb, 0xec, 0x7a, 0xac, 0xf3,
 0x19, 0x64, 0x72, 0x16, 0x9e, 0x83, 0x84, 0x30, 0x36, 0x7f, 0x66, 0xee, 0xbe,
 0x3c, 0x6e, 0x70, 0xc4, 0x16, 0xdd, 0x5f, 0x0c, 0x68, 0x75, 0x9d, 0xd1, 0xff,
 0xf8, 0x3f, 0xa4, 0x01, 0x42, 0x20, 0x9d, 0xff, 0x5e, 0xaa, 0xd9, 0x6d, 0xb9,
 0xe6, 0x38, 0x6c}, 97,
{0}, 0, SIZE_50B, PSA_ERROR_INVALID_ARGUMENT
},

{"Test psa_raw_key_agreement - Public key instead of private key\n",
 PSA_KEY_TYPE_ECC_PUBLIC_KEY(PSA_ECC_FAMILY_SECP_R1),
{0x04, 0xd1, 0x2d, 0xfb, 0x52, 0x89, 0xc8, 0xd4, 0xf8, 0x12, 0x08, 0xb7, 0x02,
 0x70, 0x39, 0x8c, 0x34, 0x22, 0x96, 0x97, 0x0a, 0x0b, 0xcc, 0xb7, 0x4c, 0x73,
 0x6f, 0xc7, 0x55, 0x44, 0x94, 0xbf, 0x63, 0x56, 0xfb, 0xf3, 0xca, 0x36, 0x6c,
 0xc2, 0x3e, 0x81, 0x57, 0x85, 0x4c, 0x13, 0xc5, 0x8d, 0x6a, 0xac, 0x23, 0xf0,
 0x46, 0xad, 0xa3, 0x0f, 0x83, 0x53, 0xe7, 0x4f, 0x33, 0x03, 0x98, 0x72, 0xab},
 65, PSA_KEY_USAGE_DERIVE, PSA_ALG_ECDH,
{0x04, 0xd1, 0x2d, 0xfb, 0x52, 0x89, 0xc8, 0xd4, 0xf8, 0x12, 0x08, 0xb7, 0x02,
 0x70, 0x39, 0x8c, 0x34, 0x22, 0x96, 0x97, 0x0a, 0x0b, 0xcc, 0xb7, 0x4c, 0x73,
 0x6f, 0xc7, 0x55, 0x44, 0x94, 0xbf, 0x63, 0x56, 0xfb, 0xf3, 0xca, 0x36, 0x6c,
 0xc2, 0x3e, 0x81, 0x57, 0x85, 0x4c, 0x13, 0xc5, 0x8d, 0x6a, 0xac, 0x23, 0xf0,
 0x46, 0xad, 0xa3, 0x0f, 0x83, 0x53, 0xe7, 0x4f, 0x33, 0x03, 0x98, 0x72, 0xab},
 65, {0}, 0, SIZE_50B, PSA_ERROR_INVALID_ARGUMENT
},
#endif
#endif
};

#ifdef ARCH_TEST_ECDH //NXP
#ifdef ARCH_TEST_ECC_CURVE_SECP256R1 //NXP
static const test_data check2[] = {
#ifdef ARCH_TEST_ECDH
#ifdef ARCH_TEST_ECC_CURVE_SECP256R1
{"Test psa_raw_key_agreement - Negative case\n",
 PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1),
{0xc8, 0x8f, 0x01, 0xf5, 0x10, 0xd9, 0xac, 0x3f, 0x70, 0xa2, 0x92, 0xda, 0xa2,
 0x31, 0x6d, 0xe5, 0x44, 0xe9, 0xaa, 0xb8, 0xaf, 0xe8, 0x40, 0x49, 0xc6, 0x2a,
 0x9c, 0x57, 0x86, 0x2d, 0x14, 0x33}, 32, PSA_KEY_USAGE_DERIVE, PSA_ALG_ECDH,
{0x04, 0xd1, 0x2d, 0xfb, 0x52, 0x89, 0xc8, 0xd4, 0xf8, 0x12, 0x08, 0xb7, 0x02,
 0x70, 0x39, 0x8c, 0x34, 0x22, 0x96, 0x97, 0x0a, 0x0b, 0xcc, 0xb7, 0x4c, 0x73,
 0x6f, 0xc7, 0x55, 0x44, 0x94, 0xbf, 0x63, 0x56, 0xfb, 0xf3, 0xca, 0x36, 0x6c,
 0xc2, 0x3e, 0x81, 0x57, 0x85, 0x4c, 0x13, 0xc5, 0x8d, 0x6a, 0xac, 0x23, 0xf0,
 0x46, 0xad, 0xa3, 0x0f, 0x83, 0x53, 0xe7, 0x4f, 0x33, 0x03, 0x98, 0x72, 0xab},
 65,
{0xd6, 0x84, 0x0f, 0x6b, 0x42, 0xf6, 0xed, 0xaf, 0xd1, 0x31, 0x16, 0xe0, 0xe1,
 0x25, 0x65, 0x20, 0x2f, 0xef, 0x8e, 0x9e, 0xce, 0x7d, 0xce, 0x03, 0x81, 0x24,
 0x64, 0xd0, 0x4b, 0x94, 0x42, 0xde}, 32, 32, PSA_SUCCESS
},
#endif
#endif
};
#endif //NXP
#endif //NXP
