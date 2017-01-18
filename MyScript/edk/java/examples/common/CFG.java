package common;

import java.io.File;

public final class CFG
{
  /**
   * Path to the MyScript SDK directory.
   */
  public static String SDK_DIR;

  /**
   * Path to the MyScript SDK resource directory.
   */
  public static final String RESOURCE_DIR;

  /**
   * Set this, e.g.: "en_GB" for English (United Kingdom).
   */
  private static final String LANG = "";

  /**
   * Set this to "iso", "hpr", "cur" or "superimposed".
   */
  private static final String HWSTYLE = "cur";

  public static final String DATA_DIR;

  public static final String OUTPUT_DIR;

  public static final String getLang()
  {
    if (CFG.LANG == null || CFG.LANG.length() == 0)
    {
      System.err.println("/!\\ please edit the CFG class and set:");
      System.err.println(" . the LANG variable");
      System.exit(-1);
    }

    return LANG;
  }

  public static final String getHwStyle()
  {
    if (CFG.HWSTYLE == null || CFG.HWSTYLE.length() == 0)
    {
      System.err.println("/!\\ please edit the CFG class and set:");
      System.err.println(" . the HWSTYLE variable");
      System.exit(-1);
    }

    return HWSTYLE;
  }

  public static final String getAkIsoRes()
  {
    return RESOURCE_DIR + '/' + getLang() + '/' + getLang() + "-ak-iso.res";
  }

  public static final String getAkHprRes()
  {
    return RESOURCE_DIR + '/' + getLang() + '/' + getLang() + "-ak-hpr.res";
  }

  public static final String getAkCurRes()
  {
    return RESOURCE_DIR + '/' + getLang() + '/' + getLang() + "-ak-cur.res";
  }

  public static final String getAkSuperimposedRes()
  {
    return RESOURCE_DIR + '/' + getLang() + '/' + getLang() + "-ak-superimposed.res";
  }

  public static final String getAkRes()
  {
    return RESOURCE_DIR + '/' + getLang() + '/' + getLang() + "-ak-" + getHwStyle() + ".res";
  }

  public static final String getLkTextRes()
  {
    return RESOURCE_DIR + '/' + getLang() + '/' + getLang() + "-lk-text.res";
  }

  public static final String getLkEnglishSecondary1Res()
  {
    return RESOURCE_DIR + "/mul/mul-lk-english_secondary_1.res";
  }

  public static final String getLkEnglishSecondary2Res()
  {
    return RESOURCE_DIR + "/mul/mul-lk-english_secondary_2.res";
  }

  public static final String getLkEnglishSecondary3Res()
  {
    return RESOURCE_DIR + "/mul/mul-lk-english_secondary_3.res";
  }

  public static final String getShapeKnowledgeRes()
  {
    return RESOURCE_DIR + "/shape/shk-standard.res";
  }

  public static final String getMathAkRes()
  {
    return RESOURCE_DIR + "/math/math-ak.res";
  }

  public static final String getMathGrmRes()
  {
    return RESOURCE_DIR + "/math/math-grm-standard.res";
  }

  public static final String getAnalyzerKnowledgeRes()
  {
    return RESOURCE_DIR + "/analyzer/ank-standard.res";
  }

  public static final String getAnalyzerShapeKnowledgeRes()
  {
    return RESOURCE_DIR + "/analyzer/shk-analyzer.res";
  }

  public static final String getMusicAkRes()
  {
    return RESOURCE_DIR + "/music/music-ak.res";
  }

  public static final String getMusicGrmRes()
  {
    return RESOURCE_DIR + "/music/music-grm-standard.res";
  }

  static
  {
    if (SDK_DIR == null || SDK_DIR.length() == 0)
    {
      File dir = new File(System.getProperty("user.dir", ""));
      
      if (dir.getPath().endsWith("edk" + File.separator + "java" + File.separator + "examples"))
      {
        SDK_DIR = "../../..";
      }
      else if (dir.getPath().endsWith("edk" + File.separator + "java"))
      {
        SDK_DIR = "../..";
      }
      else
      {
        System.err.println("/!\\ please edit the CFG class and set:");
        System.err.println(" . the SDK_DIR variable");        
      }
    }
    
    try
    {
      SDK_DIR = new File(SDK_DIR).getCanonicalPath();
    }
    catch (Throwable e)
    {
      e.printStackTrace();
      System.exit(-1);
    }
    
    RESOURCE_DIR = SDK_DIR + "/resources";
    DATA_DIR     = SDK_DIR + "/edk/examples/data/";
    
    OUTPUT_DIR = "out";
    new File(OUTPUT_DIR).mkdirs();        
  }

} // CFG
