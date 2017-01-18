package writeradaptation;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.FloatStructuredInput;
import com.myscript.engine.IFloatStroke;
import com.myscript.text.Resource;
import com.myscript.text.StructuredInputRecognizer;
import com.myscript.writeradaptation.WriterAdaptationCollector;
import com.myscript.writeradaptation.WriterAdaptationProfile;
import com.myscript.writeradaptation.WriterAdaptationTrainer;
import common.CFG;
import common.Ink;
import common.TextUIProgressBar;

/**
 * The <code>ConjointRecognitionAndAdaptation</code> example code shows you how
 * to perform conjoint handwriting recognition and adaptation.
 */
public class ConjointRecognitionAndAdaptation
{
  /**
   * The file where the writer adaptation profile will be stored.
   */
  private static final String PROFILE_FILE = CFG.OUTPUT_DIR + "/ConjointRecognitionAndAdaptation-profile.res";
  
  /**
   * Main entry point for the <code>ConjointRecognitionAndAdaptation</code> example program.
   *
   * @param args command line arguments, ignored.
   * @throws IOException 
   */
  public static final void main(String[] args) throws IOException
  {
    if (PROFILE_FILE == null || PROFILE_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for writer adaptation profile file. You can choose profile filename by editing PROFILE_FILE static member variable of ConjointRecognitionAndAdaptation class");
      System.exit(-1);
    }

    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) setting up a WriterAdaptationProfile");
    WriterAdaptationProfile profile = null;
    if (new File(PROFILE_FILE).exists())
    {
      System.out.println(" . found existing profile, loading it");
      profile = (WriterAdaptationProfile)EngineObject.load(engine, PROFILE_FILE);
    }
    else
    {
      System.out.println(" . no existing profile found, creating it");
      profile = WriterAdaptationProfile.create(engine);
    }
    System.out.println(" . profile set up successfully");
    System.out.println();

    System.out.println("3) creating and configuring a new StructuredInputRecognizer");
    final StructuredInputRecognizer recognizer = StructuredInputRecognizer.create(engine);
    System.out.println(" . recognizer created successfully");
    final Resource ak = (Resource)EngineObject.load(engine, CFG.getAkRes());
    System.out.println(" . AlphabetKnowledge resource loaded successfully");
    recognizer.attach(ak);
    System.out.println(" . AlphabetKnowledge resource attached successfully to the recognizer");
    final Resource lk = (Resource)EngineObject.load(engine, CFG.getLkTextRes());
    System.out.println(" . Main LinguisticKnowledge resource loaded successfully");
    recognizer.attach(lk);
    System.out.println(" . Main LinguisticKnowledge resource attached successfully to the recognizer");
    System.out.println(" . disposing of the main LinguisticKnowledge resource (not needed anymore)");
    lk.dispose();
    System.out.println(" . Main LinguisticKnowledge resource disposed of successfully");
    if (!CFG.getLang().contains("en"))
    {
      final String path;

      if (CFG.getLang().contains("zh") || CFG.getLang().contains("ja") || CFG.getLang().contains("ko"))
        path = CFG.getLkEnglishSecondary2Res();
      else if (CFG.getLang().contains("hi"))
        path = CFG.getLkEnglishSecondary3Res();
      else
        path = CFG.getLkEnglishSecondary1Res();

      final Resource lkEnglishSecondary = (Resource)EngineObject.load(engine, path);
      System.out.println(" . Secondary English LinguisticKnowledge resource loaded successfully");
      recognizer.attach(lkEnglishSecondary);
      System.out.println(" . Secondary English LinguisticKnowledge resource attached successfully");
      System.out.println(" . disposing of the secondary LinguisticKnowledge resource (not needed anymore)");
      lkEnglishSecondary.dispose();
      System.out.println(" . Secondary LinguisticKnowledge resource disposed of successfully");
    }
    System.out.println(" . recognizer configured successfully");
    System.out.println();

    System.out.println("4) creating a new WriterAdaptationCollector");
    final WriterAdaptationCollector collector = WriterAdaptationCollector.create(engine);
    System.out.println(" . collector created successfully");
    System.out.println();

    System.out.println("5) attaching the profile to the collector");
    collector.attach(profile);
    System.out.println(" . profile attached successfully to the collector");
    System.out.println("   . the collector will store writer data into the profile for further training");
    System.out.println();

    System.out.println("6) attaching the collector to the recognizer");
    recognizer.attach(collector);
    System.out.println(" . collector attached successfully to the recognizer");
    System.out.println("   . the recognizer will transfer incoming writer data to the collector");
    System.out.println();

    System.out.println("7) attaching the profile to the recognizer");
    recognizer.attach(profile);
    System.out.println(" . profile attached successfully to the recognizer");
    System.out.println("   . the recognizer will use existing writer data to adapt the recognition process");
    System.out.println();

    System.out.println("8) sending digital ink to the recognizer");
    final FloatStructuredInput input = FloatStructuredInput.create(engine);
    final File documentInkFile = new File(CFG.DATA_DIR + "/sentence.ink");
    Ink.parse(documentInkFile, new Ink.IParseHandler()
    {
      public void addStroke(IFloatStroke stroke)
      {
        input.addStroke(stroke);
      }
    });
    recognizer.setSource(input);
    System.out.println(" . digital ink successfully sent to the recognizer");
    System.out.println();

    System.out.println("9) running the conjoint handwriting recognition/adaptation process");
    TextUIProgressBar progressBar = new TextUIProgressBar(System.out);
    progressBar.update(0, 100);   // prints 0% status
    recognizer.run(progressBar);
    progressBar.update(100, 100); // prints 100% status
    System.out.println();
    System.out.println(" . handwriting recognition/adaptation performed successfully");
    System.out.println();

    System.out.println("10) committing the collected data");
    collector.commit();
    System.out.println(" . collected data committed successfully");
    System.out.println();

    System.out.println("11) creating and configuring a new WriterAdaptationTrainer");
    final WriterAdaptationTrainer trainer = WriterAdaptationTrainer.create(engine);
    System.out.println(" . trainer created successfully");
    trainer.attach(ak);
    System.out.println(" . AlphabetKnowledge resource attached successfully to the trainer");
    System.out.println(" . disposing of the AlphabetKnowledge resource (not needed anymore)");
    ak.dispose();
    System.out.println(" . AlphabetKnowledge resource disposed of successfully");
    trainer.attach(profile);
    System.out.println(" . profile attached successfully to the trainer");
    System.out.println(" . trainer created and configured successfully");
    System.out.println();

    System.out.println("12) running the training process on the collected data");
    TextUIProgressBar progressBarTrain = new TextUIProgressBar(System.out);
    progressBarTrain.update(0, 100);   // prints 0% status
    trainer.run(progressBarTrain);
    progressBarTrain.update(100, 100); // prints 100% status
    System.out.println();
    System.out.println(" . the trainer updated the collected writer data; next recognition will adjust itself to the writer writing style");
    System.out.println();

    System.out.println("13) storing the profile to disk");
    profile.store(PROFILE_FILE);
    System.out.println(" . profile " + new File(PROFILE_FILE).getCanonicalPath() + " stored successfully");
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("12) cleaning-up: disposing of objects and disposing of the Engine");
    input.dispose();
    trainer.dispose();
    collector.dispose();
    profile.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform writer adaptation");
  }

} // ConjointRecognitionAndAdaptation
