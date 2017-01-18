package inksearch;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.FloatStructuredInput;
import com.myscript.engine.InputUnit;
import com.myscript.inksearch.Index;
import com.myscript.inksearch.StructuredInputIndexer;
import com.myscript.text.Resource;
import common.CFG;

/**
 * The <code>StructuredInputInkIndexing</code> example code shows you how to
 * perform ink indexing on a structured input.
 */
public final class StructuredInputInkIndexing
{
    /**
     * The file where the index will be stored.
     */
    private static final String INDEX_FILE = CFG.OUTPUT_DIR + "/InkIndexing-output.index";

    /**
     * Main entry point for the <code>StructuredInputInkIndexing</code> example
     * program.
     *
     * @param args command line arguments, ignored.
     */
    public static void main(String[] args) throws IOException
    {
      if( INDEX_FILE == null || INDEX_FILE.length() == 0 )
      {
        System.err.println("/!\\ please choose a valid filename for index file. You can choose index filename by editing INDEX_FILE static member variable of StructuredInputInkIndexing class");
        System.exit(-1);
      }

      System.out.println("1) creating a new Engine initialized with your certificate");
      final Engine engine = Engine.create(MyCertificate.getBytes());
      System.out.println(" . Engine created successfully");
      System.out.println();

      System.out.println("2) creating a new StructuredInputIndexer");
      final StructuredInputIndexer indexer = StructuredInputIndexer.create(engine);
      System.out.println(" . StructuredInputIndexer created successfully");
      System.out.println();

      System.out.println("3) loading and attaching resources to the indexer");
      final Resource ak = (Resource) EngineObject.load(engine, CFG.getAkRes());
      System.out.println(" . AlphabetKnowledge resource loaded successfully");
      indexer.attach(ak);
      System.out.println(" . AlphabetKnowledge resource attached successfully");
      System.out.println(" . disposing of the AlphabetKnowledge resource (not needed anymore)");
      ak.dispose();
      System.out.println(" . AlphabetKnowledge resource disposed of successfully");

      final Resource lk = (Resource)EngineObject.load(engine, CFG.getLkTextRes());
      System.out.println(" . Main LinguisticKnowledge resource loaded successfully");
      indexer.attach(lk);
      System.out.println(" . Main LinguisticKnowledge resource attached successfully");
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
        indexer.attach(lkEnglishSecondary);
        System.out.println(" . Secondary English LinguisticKnowledge resource attached successfully");
        System.out.println(" . disposing of the secondary LinguisticKnowledge resource (not needed anymore)");
        lkEnglishSecondary.dispose();
        System.out.println(" . Secondary LinguisticKnowledge resource disposed of successfully");
      }
      System.out.println();
      
      System.out.println("4) creating a new FloatStructuredInput");
      final FloatStructuredInput input = FloatStructuredInput.create(engine);
      System.out.println(" . FloatStructuredInput created successfully");
      System.out.println();

      System.out.println("5) fueling the input with strokes");
      input.startInputUnit(InputUnit.SINGLE_LINE_TEXT);
      input.addStroke(x0, y0);
      input.addStroke(x1, y1);
      input.addStroke(x2, y2);
      input.addStroke(x3, y3);
      input.addStroke(x4, y4);
      input.addStroke(x5, y5);
      input.addStroke(x6, y6);
      input.addString(" Objects");
      input.endInputUnit(InputUnit.SINGLE_LINE_TEXT);
      System.out.println(" . input fueled successfully");
      System.out.println();

      System.out.println("6) setting the input as the source of the indexer");
      indexer.setSource(input);
      System.out.println(" . input successfully set as the indexer's source");
      System.out.println();

      System.out.println("7) running the indexing process");
      indexer.run();
      System.out.println(" . indexing performed successfully");
      System.out.println();

      System.out.println("8) retrieving the Index");
      final Index index = indexer.getResult();
      System.out.println(" . Index retrieved successfully");
      System.out.println();

      System.out.println("9) storing the Index in a file for later search");
      index.store(INDEX_FILE);
      System.out.println(" . " + new File(INDEX_FILE).getCanonicalPath() + " stored successfully");
      System.out.println();

      // do not forget the 1st rule: you create it, you dispose of it!
      System.out.println("10) cleaning-up: disposing of objects and disposing of the Engine");
      index.dispose();
      indexer.dispose();
      input.dispose();
      engine.dispose();
      System.out.println(" . clean-up done successfully");
      System.out.println();

      System.out.println("end of example:");
      System.out.println(" you just learnt how to perform ink indexing on a structured input");
    }

    /**
     * Strokes that will be indexed.
     */
    private static float[] x0 = new float[]{374, 374, 374, 374, 376, 378, 379, 381, 384, 387, 389, 390};
    private static float[] y0 = new float[]{493, 491, 493, 499, 511, 520, 522, 518, 509, 499, 493, 494};
    private static float[] x1 = new float[]{386, 388, 389, 391, 391, 391, 391, 391, 391, 394};
    private static float[] y1 = new float[]{518, 517, 516, 514, 511, 511, 513, 516, 517, 517};
    private static float[] x2 = new float[]{397, 396};
    private static float[] y2 = new float[]{502, 502};
    private static float[] x3 = new float[]{397, 398, 400, 400, 403, 404, 405, 404, 402, 400, 400, 402, 404, 406, 406, 407, 407};
    private static float[] y3 = new float[]{519, 518, 516, 514, 514, 516, 518, 520, 522, 520, 518, 516, 514, 514, 516, 518, 520};
    private static float[] x4 = new float[]{409, 409, 408};
    private static float[] y4 = new float[]{505, 503, 503};
    private static float[] x5 = new float[]{413, 414, 412, 412, 412, 414, 416, 417, 417, 415, 413};
    private static float[] y5 = new float[]{517, 516, 516, 518, 521, 522, 520, 517, 514, 513, 515};
    private static float[] x6 = new float[]{420, 420, 420, 420, 421, 422, 423, 424, 424, 425, 426, 428, 429, 430, 432, 433, 435, 435};
    private static float[] y6 = new float[]{518, 517, 515, 513, 511, 511, 511, 514, 517, 518, 518, 516, 514, 514, 515, 518, 519, 519};

} // StructuredInputInkIndexing
