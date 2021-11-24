// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2020 The sbercoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x000003bc45fd0164261390e1e69a60179532c39bc4ec652371dd87fd5210a2b5"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1637557200, // * UNIX timestamp of last checkpoint block
    0,    		// * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    1440        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    0};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x4D;
        pchMessageStart[1] = 0x47;
        pchMessageStart[2] = 0x54;
        pchMessageStart[3] = 0x4D;
        vAlertPubKey = ParseHex("0493e2b829f747ad551c64a223d3842402894db1a9b4cbb53634ed6d375a7b4e58036f2085e35c85c39dfe986beb636bb9059f488eeb3faa4d9faa047ba3796101"); // mainalert pub
        nDefaultPort = 22220;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // sbercoin starting difficulty is 1 / 2^12
        nMaxReorganizationDepth = 100;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 60;  // sbercoin: 1 minutes
        nMaturity = 110;
        nMaxMoneyOut = 33500000000000 * COIN;

        //nSubsidyHalvingBlock = 0;
        //nMasternodeCollateral = 6000;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 250;
        nModifierUpdateBlock = 2147483646;
        nZerocoinStartHeight = 251;
        nZerocoinStartTime = 1637557200; // 1637557200 (timestamp genesis block) + 251 (block) * 60 (timespan block) =~ 1637557200
        nBlockZerocoinV2 = 2147483646;

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "Mon Nov 22 2021 05:00:00 GMT+0000 - start blockchain SBR.";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04b8e31af23f97791f149cf46fd3f87e1d67109b24beed6a9425e4dfd73d371e14563290e6b22da9ac90fdef23f5a0ad3613da72baa06578031a367d1ce0300539") << OP_CHECKSIG; // main pub
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1637557200;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 936662;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000003bc45fd0164261390e1e69a60179532c39bc4ec652371dd87fd5210a2b5"));
        assert(genesis.hashMerkleRoot == uint256("0x1ca05b5aaa7addc4ef224ffad9786f7e69f6a8c25eda8d0edb9f72e4d4f919a7")); // 0x {merkle hash}

        vSeeds.push_back(CDNSSeedData("45.61.51.226", "45.61.51.226"));
        vSeeds.push_back(CDNSSeedData("45.61.49.98", "45.61.49.98"));
        vSeeds.push_back(CDNSSeedData("194.37.82.193", "194.37.82.193"));
        vSeeds.push_back(CDNSSeedData("185.227.108.23", "185.227.108.23"));
        vSeeds.push_back(CDNSSeedData("45.61.50.11", "45.61.50.11"));
        vSeeds.push_back(CDNSSeedData("185.144.159.201", "185.144.159.201"));
        vSeeds.push_back(CDNSSeedData("103.125.218.250", "103.125.218.250"));
        vSeeds.push_back(CDNSSeedData("45.61.49.103", "45.61.49.103"));

        // sbercoin addresses start with 'С'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 63); // sbercoin addresses start with 'K' https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 30);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 128+97);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04ab3fffbe9cf7cb0428f06a2ff70f7fbf008ca2a9bcc838d2d8a4f6c5e791750464e6646b9cbbfcba24cf66d981c1d023f70f88df5819e3737581ea36179fc860"; // mainspork pub
        strObfuscationPoolDummyAddress = "SfxwVYWkfsfsR78KFcim2qa9o4Vwik35dB";
        nStartMasternodePayments = 1637563800;

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zsbercoin to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x54;
        pchMessageStart[1] = 0x47;
        pchMessageStart[2] = 0x54;
        pchMessageStart[3] = 0x4D;
        vAlertPubKey = ParseHex("045c9dfe71c1f7dc9709831030d102800784a81e73cc8e98800234958d4457bde0b71d76f3220f22591e0a8ab30b635b27b9fa7c7454d8f0ad7a749364b3b8e26b"); // testnetalert pub
        nDefaultPort = 22221;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 60;
        nLastPOWBlock = 200;
        nMaturity = 15;
        nModifierUpdateBlock = 0;
        nMaxMoneyOut = 21000000 * COIN;
        nZerocoinStartHeight = 15;
        nZerocoinStartTime = 1637557200;
        nBlockZerocoinV2 = 15;

        //nSubsidyHalvingBlock = 1600;
        //nMasternodeCollateral = 6000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1637557200;
        genesis.nNonce = 936662;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000003bc45fd0164261390e1e69a60179532c39bc4ec652371dd87fd5210a2b5"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 63); // Testnet sbercoin addresses start with 'K' https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 98);  // Testnet sbercoin script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 128+98);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "0436360d086126b01873d8f9c96872366d6fdc914e7afc08f029f0010765bb86374ee799b5bd762c25343200fa97d260b9aff207f8ca2a73580792b6e6876c5ac3"; // testnetspork pub
        strObfuscationPoolDummyAddress = "";
        nStartMasternodePayments = 1637557200;
        nBudget_Fee_Confirmations = 3;

    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x52;
        pchMessageStart[1] = 0x47;
        pchMessageStart[2] = 0x54;
        pchMessageStart[3] = 0x4D;
        nMinerThreads = 1;
        nTargetSpacing = 1 * 60;
        nDefaultPort = 22222;

        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1637557200;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 936662;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000003bc45fd0164261390e1e69a60179532c39bc4ec652371dd87fd5210a2b5"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 22223;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    //virtual void setSubsidyHalvingBlock(int anSubsidyHalvingBlock) { nSubsidyHalvingBlock = anSubsidyHalvingBlock; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
