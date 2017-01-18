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
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.geometry.Rectangle;
import com.myscript.ink.SelectionModifier;
import com.myscript.shape.ShapeDocument;
import com.myscript.shape.ShapeKnowledge;
import com.myscript.shape.ShapeRecognizer;
import com.myscript.shape.ShapeSegment;
import common.CFG;
import common.TextUIProgressBar;

/**
 * The <code>ShapeRecognition</code> example code shows you how to perform a shape
 * recognition on a document.
 */
public class ShapeRecognition
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
   * Main entry point for the <code>ShapeRecognition</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of ShapeRecognition class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of ShapeRecognition class");
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

    System.out.println("4) adding a new shape field to the content of the first page");
    ContentField shapeField = content.addNewField(ContentFieldType.SHAPE,
                                                 "myShapeField");
    System.out.println(" . shape field added successfully");
    System.out.println("");

    System.out.println("5) selecting a rectangular part of the first page");
    final PageSelection selection = PageSelection.create(page);
    final Rectangle shapeRectangle = new Rectangle(160.0f, 661.0f, 1700.0f, 900.0f);
    selection.selectRectangle(shapeRectangle, SelectionModifier.SET);
    System.out.println(" . selection done successfully");
    System.out.println("");

    System.out.println("6) adding the selection to the shape field");
    shapeField.addItems(selection);
    System.out.println(" . selection added to the shape field successfully");
    System.out.println("");

    System.out.println("7) creating and configuring a new shapeRecognizer");
    final ShapeRecognizer shapeRecognizer = ShapeRecognizer.create(engine);
    System.out.println(" . ShapeRecognizer created successfully");
    final ShapeKnowledge shapeKnowledge = (ShapeKnowledge)EngineObject.load(engine, CFG.getShapeKnowledgeRes());
    System.out.println(" . ShapeKnowledge loaded successfully");
    shapeRecognizer.attach(shapeKnowledge);
    shapeKnowledge.dispose();
    System.out.println(" . ShapeKnowledge attached to the recognizer successfully");
    System.out.println("");

    System.out.println("8) performing a recognition on the selection");
    final TextUIProgressBar progressBar = new TextUIProgressBar(System.out);
    progressBar.update(0, 100); // prints 0% status
    shapeRecognizer.process(shapeField, progressBar);
    progressBar.update(100, 100); // prints 100% status
    System.out.println("");
    System.out.println(" . recognition performed successfully");
    System.out.println("");

    System.out.println("9) retrieving the recognition result");
    final ShapeDocument shapeResult = (ShapeDocument)shapeField.getResult();
    System.out.println(" . result retrieved successfully");
    
    final int segmentCount = shapeResult.getSegmentCount();
    System.out.println(" . retrieved " + segmentCount + " segments");
    {
      for (int segmentId = 0; segmentId < segmentCount; ++segmentId)
      {
        ShapeSegment shapeSegment = shapeResult.getSegmentAt(segmentId);
        int candidateCount = shapeSegment.getCandidateCount();
        System.out.println("   . ShapeSegment #" + segmentId + " has " + candidateCount + " ShapeCandidate" + (candidateCount > 1 ? "s" : ""));
        shapeSegment.dispose();
      }
    }
    System.out.println("");

    System.out.println("10) cleaning-up: releasing all created objects and destroying the Engine");
    shapeResult.dispose();
    shapeRecognizer.dispose();
    shapeKnowledge.dispose();
    selection.dispose();
    shapeField.dispose();

    content.dispose();
    page.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println("");

    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform a shape recognition on a document");
  }

} // ShapeRecognition
