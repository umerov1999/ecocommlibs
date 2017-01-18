package analyzer;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;

import certificates.MyCertificate;

import com.myscript.analyzer.Analyzer;
import com.myscript.analyzer.AnalyzerDocument;
import com.myscript.analyzer.AnalyzerKnowledge;
import com.myscript.edk.java.analyzer.DocumentToSVG;
import com.myscript.edk.java.analyzer.DocumentToSVG.DocumentConfig;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.IFloatStroke;
import com.myscript.shape.ShapeBeautifier;
import com.myscript.shape.ShapeKnowledge;
import com.myscript.shape.ShapeRecognizer;
import com.myscript.text.Resource;
import com.myscript.text.StructuredInputRecognizer;
import common.CFG;
import common.Ink;

/**
 * The <code>DocumentAnalysis</code> example code shows you how to analyze a
 * handwritten document.
 */
public class DocumentAnalysis
{
  /**
   * Main entry point for the <code>DocumentAnalysis</code> example program.
   *
   * @param args command line arguments, ignored.
   * @throws IOException 
   */
  public static final void main(String[] args) throws IOException
  {
    // SVG styling
    DocumentConfig config = new DocumentConfig();
    // we use an output box of 630x891, which is close to 600x800 but with A* page
    // format series ratio, with a margin of 5%
    config.docWidth = 630.0f;
    config.docHeight = 891.0f;
    config.docMargin = 0.05f;
    config.strokeColor = "black";
    config.strokeWidth = 2.0f;
    config.textHeightFactor = 1.25f;
    config.displayOriginalInk = true;
    config.originalInkColor = "grey";
    config.arrowShapeFactor = 2.0f;
    config.arrowSizeFactor = 0.08f;

    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new Analyzer");
    final Analyzer analyzer = Analyzer.create(engine);
    System.out.println(" . Analyzer created successfully");
    System.out.println();

    System.out.println("3) loading and attaching resource to the Analyzer");
    final AnalyzerKnowledge ank = (AnalyzerKnowledge)EngineObject.load(engine, CFG.getAnalyzerKnowledgeRes());
    System.out.println(" . AnalyzerKnowledge resource loaded successfully");
    analyzer.attach(ank);
    System.out.println(" . AnalyzerKnowledge resource attached successfully");
    System.out.println(" . disposing of the AnalyzerKnowledge resource (not needed anymore)");
    ank.dispose();
    System.out.println(" . AnalyzerKnowledge resource disposed of successfully");
    System.out.println();

    System.out.println("4) creating and configuring a new StructuredInputRecognizer");
    final StructuredInputRecognizer textRecognizer = StructuredInputRecognizer.create(engine);
    System.out.println(" . StructuredInputRecognizer created successfully");
    System.out.println(" . loading and attaching resources to the StructuredInputRecognizer");
    final Resource ak = (Resource)EngineObject.load(engine, CFG.getAkRes());
    System.out.println(" . AlphabetKnowledge resource loaded successfully");
    textRecognizer.attach(ak);
    System.out.println(" . AlphabetKnowledge resource attached successfully");
    System.out.println(" . disposing of the AlphabetKnowledge resource (not needed anymore)");
    ak.dispose();
    System.out.println(" . AlphabetKnowledge resource disposed of successfully");
    final Resource lk = (Resource)EngineObject.load(engine, CFG.getLkTextRes());
    System.out.println(" . Main LinguisticKnowledge resource loaded successfully");
    textRecognizer.attach(lk);
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
      textRecognizer.attach(lkEnglishSecondary);
      System.out.println(" . Secondary English LinguisticKnowledge resource attached successfully");
      System.out.println(" . disposing of the secondary LinguisticKnowledge resource (not needed anymore)");
      lkEnglishSecondary.dispose();
      System.out.println(" . Secondary LinguisticKnowledge resource disposed of successfully");
    }
    System.out.println();

    System.out.println("5) creating and configuring a new ShapeRecognizer and a new ShapeBeautifier");
    final ShapeRecognizer shapeRecognizer = ShapeRecognizer.create(engine);
    System.out.println(" . ShapeRecognizer created successfully");
    final ShapeBeautifier shapeBeautifier = ShapeBeautifier.create(engine);
    System.out.println(" . ShapeBeautifier created successfully");
    System.out.println(" . loading and attaching the ShapeKnowledge to the ShapeRecognizer and to the ShapeBeautifier");
    final ShapeKnowledge shk = (ShapeKnowledge)EngineObject.load(engine, CFG.getAnalyzerShapeKnowledgeRes());
    System.out.println(" . ShapeKnowledge resource loaded successfully");
    shapeRecognizer.attach(shk);
    System.out.println(" . ShapeKnowledge resource attached successfully to the ShapeRecognizer");
    shapeBeautifier.attach(shk);
    System.out.println(" . ShapeKnowledge resource attached successfully to the ShapeBeautifier");
    System.out.println(" . disposing of the ShapeKnowledge resource (not needed anymore)");
    shk.dispose();
    System.out.println(" . ShapeKnowledge resource disposed of successfully");
    System.out.println();

    System.out.println("6) attaching the StructuredInputRecognizer and the ShapeRecognizer and the ShapeBeautifier to the Analyzer");
    analyzer.attach(textRecognizer);
    System.out.println(" . StructuredInputRecognizer attached successfully to the Analyzer");
    analyzer.attach(shapeRecognizer);
    System.out.println(" . ShapeRecognizer attached successfully to the Analyzer");
    analyzer.attach(shapeBeautifier);
    System.out.println(" . ShapeBeautifier attached successfully to the Analyzer");
    System.out.println();

    System.out.println("7) creating a new AnalyzerDocument");
    final AnalyzerDocument document = AnalyzerDocument.create(engine);
    System.out.println(" . AnalyzerDocument created successfully");
    System.out.println();

    System.out.println("8) fueling the AnalyzerDocument with digital ink");
    final File documentInkFile = new File(CFG.DATA_DIR + "/document.ink");
    Ink.parse(documentInkFile, new Ink.IParseHandler()
    {
      public void addStroke(IFloatStroke stroke)
      {
        document.addStroke(stroke);
      }
    });
    System.out.println(" . AnalyzerDocument successfully fueled with digital ink");
    System.out.println();

    System.out.println("9) running the document analysis process");
    analyzer.process(document);
    System.out.println(" . document analysis performed successfully");
    System.out.println();

    System.out.println("10) exploring the analyzed document");
    int strokeCount = document.getStrokeCount();
    if (strokeCount > 0)
    {
      final String path = new File(CFG.OUTPUT_DIR + "/DocumentAnalysis-output.svg").getCanonicalPath();
      final PrintWriter out = new PrintWriter(path);
      out.print(DocumentToSVG.format(document, config));
      out.close();
      
      System.out.println(" . analyzed document stored as SVG: " + path);
      System.out.println();
    }
    else
    {
      System.out.println(" . no analysis result");
      System.out.println();
    }

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("11) cleaning-up: disposing of objects and disposing of the Engine");
    document.dispose();
    shapeBeautifier.dispose();
    shapeRecognizer.dispose();
    textRecognizer.dispose();
    analyzer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform a document analysis");
  }

} // DocumentAnalysis
