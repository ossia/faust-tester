#include <cmath>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <string_view>
#include <faust/dsp/llvm-dsp.h>

#if _WIN32
#include <windows.h> // for EXCEPTION_ACCESS_VIOLATION
#include <excpt.h>
#endif

#define CHECK(x) do { \
  if(!(x)) { \
    std::cerr << "ERROR: " << logger.str() << "\nLine " <<  __LINE__ << " : !" << #x << " => "; \
    return false; \
  } \
} while(0)


const int max_channels = 64;
const int buffer_size = 512;
std::vector<FAUSTFLOAT> in_v;
std::vector<FAUSTFLOAT> out_v;
bool faust_main(int argc, const char** argv, const std::string& path)
{
  int sr = 48000;
  //for(int sr : {44100, 48000, 88200, 96000, 192000})
  {
    std::stringstream logger;
    logger << path << " SR:" << sr << " ... ";
    std::string err;
    err.resize(4097);
    
#if defined(_WIN32)
    __try {
#endif
    
    llvm_dsp_factory* fac{}; 
    fac = createDSPFactoryFromFile(path, argc, argv, "", err, -1);
    if(!fac)
    {
      std::cerr << "ERROR (compiler): " << logger.str() << "\n" << err.c_str();
      return false;
    }
    CHECK(fac);
    
    logger << " Create? ";
    auto obj = fac->createDSPInstance();
    CHECK(obj);

    logger << " Init? ";
    obj->init(sr);
    const int faust_ins = obj->getNumInputs();    
    const int faust_outs = obj->getNumOutputs();    
    const int faust_sr = obj->getSampleRate();
    CHECK(faust_ins < max_channels);
    CHECK(faust_outs < max_channels);
    CHECK(sr == faust_sr);
    
    FAUSTFLOAT* inputs[max_channels];
    FAUSTFLOAT* outputs[max_channels];
    out_v.clear();
    out_v.resize(max_channels * buffer_size);
    for(int i = 0; i < max_channels; i++)
    {
      inputs[i] = in_v.data() + i * buffer_size;
      outputs[i] = out_v.data() + i * buffer_size;
    }
    
    obj->compute(512, inputs, outputs);
    
    for(int c = 0; c < faust_outs; c++)
    for(int i = 0; i < 512; i++)
    {
      CHECK(!std::isnan(outputs[c][i]));
      CHECK(!std::isinf(outputs[c][i]));
    }
    
    logger << " Delete obj? ";
    delete obj;
    
    logger << " Delete fac? ";
    deleteDSPFactory(fac);
    logger << "\n";
    
#if defined(_WIN32)
    } __except(true) {      
      std::cerr << "SEGFAULT: " << GetExceptionCode() << ": " << logger.str() << "\n" << err.c_str() << "\n";
    }
#endif
  }
  return true;
}
int main(int argc, char** argv)
{
  using namespace std;
  std::filesystem::path filename = argv[1];
  
  // Init ffunction
  
  // cmath
  registerForeignFunction("signbit");
  
  // Guitarix functions
  registerForeignFunction("Ftube");
  registerForeignFunction("Ranode");
  registerForeignFunction("Ftrany");
  registerForeignFunction("asymclip");
  registerForeignFunction("asymclip2");
  registerForeignFunction("asymclip3");
  registerForeignFunction("opamp");
  registerForeignFunction("opamp1");
  registerForeignFunction("opamp2");
  registerForeignFunction("asymhardclip");
  registerForeignFunction("asymhardclip2");
  registerForeignFunction("symclip"); 
  
  registerForeignFunction("epiphone_jr_out_negclip");
  registerForeignFunction("epiphone_jr_outclip");
  registerForeignFunction("orangedarkterrorp3_negclip");
  registerForeignFunction("orangedarkterrorp3clip");
  registerForeignFunction("plexipowerampel34_negclip");
  registerForeignFunction("plexipowerampel34clip");
  registerForeignFunction("princetonclip");
  registerForeignFunction("supersonic_negclip");
  registerForeignFunction("supersonicclip");
  registerForeignFunction("tweedchamp_negclip");
  registerForeignFunction("tweedchampclip");

  // STK functions
  registerForeignFunction("loadPreset");
  registerForeignFunction("loadPhonemeGains");
  registerForeignFunction("loadPhonemeParameters");
  registerForeignFunction("readMarmstk1");
  registerForeignFunction("getValueDryTapAmpT60piano");
  registerForeignFunction("getValueSustainPedalLevel");
  registerForeignFunction("getValueLoudPole");
  registerForeignFunction("getValuePoleValue");
  registerForeignFunction("getValueLoudGain");
  registerForeignFunction("getValueSoftGain");
  registerForeignFunction("getValueDCBa1piano");
  registerForeignFunction("getValuer1_1db");
  registerForeignFunction("getValuer1_2db");
  registerForeignFunction("getValuer2db");
  registerForeignFunction("getValuer3db");
  registerForeignFunction("getValueSecondStageAmpRatio");
  registerForeignFunction("getValueSecondPartialFactor");
  registerForeignFunction("getValueThirdPartialFactor");
  registerForeignFunction("getValueBq4_gEarBalled");
  registerForeignFunction("getValueStrikePosition");
  registerForeignFunction("getValueEQBandWidthFactor");
  registerForeignFunction("getValueEQGain");
  registerForeignFunction("getValueDetuningHz");
  registerForeignFunction("getValueSingleStringDecayRate");
  registerForeignFunction("getValueSingleStringZero");
  registerForeignFunction("getValueSingleStringPole");
  registerForeignFunction("getValueStiffnessCoefficient");
  registerForeignFunction("getValueReleaseLoopGainpiano");
  registerForeignFunction("getValueLoopFilterb0piano");
  registerForeignFunction("getValueLoopFilterb1piano");
  registerForeignFunction("getValueLoopFilterb2piano");
  registerForeignFunction("getValueLoopFiltera1piano");
  registerForeignFunction("getValueLoopFiltera2piano");
  registerForeignFunction("getValueBassLoopFilterb0piano");
  registerForeignFunction("getValueBassLoopFilterb1piano");
  registerForeignFunction("getValueBassLoopFiltera1piano");
  registerForeignFunction("getValueDryTapAmpT60harpsichord");
  registerForeignFunction("getValueReleaseLoopGainharpsichord");
  registerForeignFunction("getValueLoopFilterb0harpsichord");
  registerForeignFunction("getValueLoopFilterb1harpsichord");
  registerForeignFunction("getValueLoopFilterb2harpsichord");
  registerForeignFunction("getValueLoopFiltera1harpsichord");
  registerForeignFunction("getValueLoopFiltera2harpsichord");
  registerForeignFunction("getValueBassLoopFilterb0");
  registerForeignFunction("getValueBassLoopFilterb1bass");
  registerForeignFunction("getValueBassLoopFiltera1bass");
  
  // Init buffer
  in_v.resize(max_channels * buffer_size);
  int k = 0;
  for(auto& f: in_v)
  {
    f = sin(k++);
  }
  
  std::string path = filename.string();
  std::string folder = filename.parent_path().string();
  std::vector<const char*> args;
  
  args.push_back("-I"); 
#define STRINGIZE_(s) #s
#define STRINGIZE(s) STRINGIZE_(s)
  args.push_back(STRINGIZE(FAUST_LIBS_ROOT));
  args.push_back("-I");
  args.push_back(folder.c_str());
  
  
  if (sizeof(FAUSTFLOAT) == 4)
  {
    {
      auto a = args;
      a.push_back("-single");
      bool ok = faust_main(a.size(), a.data(), path);
      if(!ok) std::cerr << " single failed!\n";
    }      
    {
      auto a = args;
      a.push_back("-vec");
      a.push_back("-single");
      bool ok = faust_main(a.size(), a.data(), path);
      if(!ok) std::cerr << " vec single failed!\n";
    }  
  }
  else
  {
    { 
      auto a = args;
      a.push_back("-double");
      bool ok = faust_main(a.size(), a.data(), path);
      if(!ok) std::cerr << " double failed! \n";
    }
    
    {
      auto a = args;
      a.push_back("-vec");
      a.push_back("-double");
      bool ok = faust_main(a.size(), a.data(), path);
      if(!ok) std::cerr << " vec double failed! \n";
    }
  }
}
