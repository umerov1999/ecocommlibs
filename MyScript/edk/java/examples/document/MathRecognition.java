package document;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.document.AsyncCommand;
import com.myscript.document.Content;
import com.myscript.document.ContentField;
import com.myscript.document.ContentFieldType;
import com.myscript.document.Document;
import com.myscript.document.IAsyncCommandHandler;
import com.myscript.document.Page;
import com.myscript.document.PageSelection;
import com.myscript.edk.java.math.ParseTreeToLaTeX;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.geometry.Circle;
import com.myscript.ink.SelectionModifier;
import com.myscript.math.MathAlphabetKnowledge;
import com.myscript.math.MathGrammar;
import com.myscript.math.MathNode;
import com.myscript.math.MathRecognizer;
import common.CFG;
import common.TextUIProgressBar;

/**
 * The <code>MathRecognition</code> example code shows you how to perform an
 * math recognition on a document.
 */
public class MathRecognition
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
   * Main entry point for the <code>MathRecognition</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of MathRecognition class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of MathRecognition class");
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

    System.out.println("3) getting the second page of the Document and its content");
    final Page page1 = document.getPageAt(1);
    System.out.println(" . second page retrieved successfully");
    final Content[] contentHandler = new Content[1];
    page1.getContent(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to load page content");
        contentHandler[0] = (Content)value.acquire();
      }
    });
    page1.sync(); // wait for getContent() to complete
    final Content content = contentHandler[0];
    System.out.println(" . second page's content retrieved successfully");
    System.out.println("");

    System.out.println("4) adding a new math field to the content of the second page");
    final ContentField mathField = content.addNewField(ContentFieldType.MATH, "myMathField");
    System.out.println(" . math field added successfully");
    System.out.println("");

    System.out.println("5) selecting a circular part of the second page");
    final PageSelection selection = PageSelection.create(page1);
    final Circle mathCircle = new Circle(750.0f, 450.0f, 350.0f);
    selection.selectCircle(mathCircle, SelectionModifier.SET);
    System.out.println(" . selection done successfully");
    System.out.println("");

    System.out.println("6) adding the selection to the math field");
    mathField.addItems(selection);
    System.out.println(" . selection added to the math field successfully");
    System.out.println("");

    System.out.println("7) creating and configuring a new MathRecognizer");
    final MathRecognizer mathRecognizer = MathRecognizer.create(engine);
    System.out.println(" . MathRecognizer created successfully");
    final MathAlphabetKnowledge mathAlphabetKnowledge = (MathAlphabetKnowledge)EngineObject.load(engine, CFG.getMathAkRes());
    System.out.println(" . MathAlphabetKnowledge loaded successfully");
    final MathGrammar mathGrammar = (MathGrammar)EngineObject.load(engine, CFG.getMathGrmRes());
    System.out.println(" . MathGrammar loaded successfully");
    mathRecognizer.attach(mathAlphabetKnowledge);
    mathRecognizer.attach(mathGrammar);
    mathAlphabetKnowledge.dispose();
    mathGrammar.dispose();
    System.out.println(" . MathAlphabetKnowledge and MathGrammar resources attached to the recognizer successfully");
    System.out.println("");

    System.out.println("8) performing a recognition on the selection");
    final TextUIProgressBar progressBar = new TextUIProgressBar(System.out);
    progressBar.update(0, 100); // prints 0% status
    mathRecognizer.process(mathField, progressBar);
    progressBar.update(100, 100); // prints 100% status
    System.out.println("");
    System.out.println(" . recognition performed successfully");
    System.out.println("");

    System.out.println("9) retrieving the recognition result");
    final MathNode root = (MathNode)mathField.getResult();
    if (root != null)
    {
      System.out.println(" . result retrieved successfully");
      System.out.println(" . math recognized as LaTeX string: ");
      System.out.println();
      System.out.println(ParseTreeToLaTeX.format(root));
      root.dispose();
    }
    else
    {
      System.out.println(" . no recognition result");
    }
    System.out.println("");

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("10) cleaning-up: disposing of objects and disposing of the Engine");
    mathRecognizer.dispose();
    selection.dispose();
    mathField.dispose();
    content.dispose();
    page1.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform an math recognition on a document");
  }

} // MathRecognition
