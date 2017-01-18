package document;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;

import certificates.MyCertificate;

import com.myscript.analyzer.Analyzer;
import com.myscript.analyzer.AnalyzerDocument;
import com.myscript.analyzer.AnalyzerKnowledge;
import com.myscript.document.AsyncCommand;
import com.myscript.document.Content;
import com.myscript.document.ContentField;
import com.myscript.document.ContentFieldType;
import com.myscript.document.Document;
import com.myscript.document.IAsyncCommandHandler;
import com.myscript.document.Page;
import com.myscript.document.PageSelection;
import com.myscript.edk.java.analyzer.DocumentToSVG;
import com.myscript.edk.java.analyzer.DocumentToSVG.DocumentConfig;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.shape.ShapeBeautifier;
import com.myscript.shape.ShapeKnowledge;
import com.myscript.shape.ShapeRecognizer;
import com.myscript.text.Resource;
import com.myscript.text.StructuredInputRecognizer;
import common.CFG;
import common.TextUIProgressBar;

/**
 * The <code>DocumentAnalysis</code> example code shows you how to perform an
 * analysis on a document.
 */
public class DocumentAnalysis
{ 
  /**
   * The temporary directory used in this example.
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  
  /**
   * The document file opened to load its ink. (Note that you can rename any
   * notes document as a zip file to watch inside it.)
   */
  private static final String DOCUMENT_FILE = CFG.DATA_DIR + "/example1.notes";

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
    
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of DocumentAnalysis class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of DocumentAnalysis class");
      System.exit(-1);
    }

    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) loading an existing Document");
    final Document document = (Document)EngineObject.load(engine, DOCUMENT_FILE);
    document.setTemporaryDirectory(TEMPORARY_DIRECTORY);
    System.out.println(" . " + new File(DOCUMENT_FILE).getCanonicalPath() + " loaded successfully");
    System.out.println();

    System.out.println("3) getting the first page of the Document and its content");
    final Page page = document.getPageAt(0);
    System.out.println(" . first page retrieved successfully");
    final Content[] contentHandler = new Content[1];
    page.getContent(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to load page content");
        contentHandler[0] = (Content)value.acquire();
      }
    });
    page.sync(); // wait for getContent() to complete
    final Content content = contentHandler[0];
    System.out.println(" . first page's content retrieved successfully");
    System.out.println("");

    System.out.println("4) adding a new analyzer field to the content of the first page");
    final ContentField analyzerField = content.addNewField(ContentFieldType.ANALYZER, "myAnalyzerField");
    System.out.println(" . analyzer field added successfully");
    System.out.println("");

    System.out.println("5) selecting all the first page");
    final PageSelection selection = PageSelection.create(page);
    selection.selectAll();
    System.out.println(" . selection done successfully");
    System.out.println("");

    System.out.println("6) adding the selection to the analyzer field");
    analyzerField.addItems(selection);
    System.out.println(" . selection added to the analyzer field successfully");
    System.out.println("");
    
    System.out.println("7) creating and configuring a new Analyzer");
    final Analyzer analyzer = Analyzer.create(engine);
    System.out.println(" . Analyzer created successfully");
    final AnalyzerKnowledge analyzerKnowledge = (AnalyzerKnowledge)EngineObject.load(engine, CFG.getAnalyzerKnowledgeRes());
    System.out.println(" . AnalyzerKnowledge loaded successfully");
    analyzer.attach(analyzerKnowledge);
    System.out.println(" . AnalyzerKnowledge resource attached to the Analyzer successfully");
    analyzerKnowledge.dispose();
    System.out.println(" . disposed of AnalyzerKnowledge resource");
    System.out.println("");
    
    System.out.println("8) creating and configuring a new StructuredInputRecognizer");
    final StructuredInputRecognizer textRecognizer = StructuredInputRecognizer.create(engine);
    System.out.println(" . StructuredInputRecognizer created successfully");
    final Resource ak = (Resource)EngineObject.load(engine, CFG.getAkRes());
    System.out.println(" . AlphabetKnowledge resource loaded successfully");
    final Resource lk = (Resource)EngineObject.load(engine, CFG.getLkTextRes());
    System.out.println(" . LinguisticKnowledge resource loaded successfully");
    textRecognizer.attach(ak);
    textRecognizer.attach(lk);
    ak.dispose();
    lk.dispose();
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
      lkEnglishSecondary.dispose();
    }
    System.out.println(" . AlphabetKnowledge and LinguisticKnowledge resources attached to the recognizer successfully");
    System.out.println("");

    System.out.println("9) creating and configuring a new ShapeRecognizer and a new ShapeBeautifier");
    final ShapeRecognizer shapeRecognizer = ShapeRecognizer.create(engine);
    System.out.println(" . ShapeRecognizer created successfully");
    final ShapeBeautifier shapeBeautifier = ShapeBeautifier.create(engine);
    System.out.println(" . ShapeBeautifier created successfully");
    final ShapeKnowledge shapeKnowledge = (ShapeKnowledge)EngineObject.load(engine, CFG.getAnalyzerShapeKnowledgeRes());
    System.out.println(" . ShapeKnowledge resource loaded successfully");
    shapeRecognizer.attach(shapeKnowledge);
    shapeBeautifier.attach(shapeKnowledge);
    System.out.println(" . ShapeKnowledge resource attached to the recognizer and the beautifier successfully");
    shapeKnowledge.dispose();
    System.out.println("");

    System.out.println("10) attaching the StructuredInputRecognizer, the ShapeRecognizer and the ShapeBeautifier to the Analyzer");
    analyzer.attach(textRecognizer);
    System.out.println(" . StructuredInputRecognizer attached to the Analyzer successfully");
    analyzer.attach(shapeRecognizer);
    System.out.println(" . ShapeRecognizer attached to the Analyzer successfully");
    analyzer.attach(shapeBeautifier);
    System.out.println(" . ShapeBeautifier attached to the Analyzer successfully");
    System.out.println("");

    System.out.println("11) running the document analysis process");
    final TextUIProgressBar progressBar = new TextUIProgressBar(System.out);
    progressBar.update(0, 100); // prints 0% status
    analyzer.process(analyzerField, progressBar);
    progressBar.update(100, 100); // prints 100% status
    System.out.println("");
    System.out.println(" . document analysis performed successfully");
    System.out.println("");

    System.out.println("12) retrieving the analysis result");
    final AnalyzerDocument analyzerResult = (AnalyzerDocument)analyzerField.getResult();
    System.out.println(" . analysis result retrieved successfully");
    System.out.println("");

    System.out.println("13) exploring the analyzed document");
    int strokeCount = analyzerResult.getStrokeCount();
    if (strokeCount > 0)
    {
      final String path = new File(CFG.OUTPUT_DIR + "/DocumentAnalysis-output.svg").getCanonicalPath();
      final PrintWriter out = new PrintWriter(path);
      out.print(DocumentToSVG.format(analyzerResult, config));
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
    System.out.println("14) cleaning-up: disposing of objects and disposing of the Engine");
    analyzerResult.dispose();
    shapeBeautifier.dispose();
    shapeRecognizer.dispose();
    textRecognizer.dispose();
    analyzer.dispose();
    analyzerField.dispose();
    content.dispose();
    page.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println("");

    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform an analysis on a document");
  }

} // DocumentAnalysis
