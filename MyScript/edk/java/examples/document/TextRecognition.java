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
import com.myscript.text.CandidateIterator;
import com.myscript.text.RecognitionResult;
import com.myscript.text.Resource;
import com.myscript.text.TextRecognizer;
import common.CFG;
import common.TextUIProgressBar;

/**
 * The <code>TextRecognition</code> example code shows you how to perform a text
 * recognition on a document.
 */
public class TextRecognition
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
   * Main entry point for the <code>TextRecognition</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of TextRecognition class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of TextRecognition class");
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

    System.out.println("4) adding a new text field to the content of the first page");
    final ContentField textField = content.addNewField(ContentFieldType.TEXT, "myTextField");
    System.out.println(" . text field added successfully");
    System.out.println("");

    System.out.println("5) selecting a rectangular part of the first page");
    final PageSelection selection = PageSelection.create(page);
    final Rectangle textRectangle = new Rectangle(100.0f, 300.0f, 1780.0f, 360.0f);
    selection.selectRectangle(textRectangle, SelectionModifier.SET);
    System.out.println(" . selection done successfully");
    System.out.println("");

    System.out.println("6) adding selection to the text field");
    textField.addItems(selection);
    System.out.println(" . selection added to the text field successfully");
    System.out.println("");

    System.out.println("7) creating and configuring a new TextRecognizer");
    final TextRecognizer textRecognizer = TextRecognizer.create(engine);
    System.out.println(" . TextRecognizer created successfully");
    final Resource ak = (Resource)EngineObject.load(engine, CFG.getAkCurRes());
    System.out.println(" . AlphabetKnowledge loaded successfully");
    final Resource lk = (Resource)EngineObject.load(engine, CFG.getLkTextRes());
    System.out.println(" . LinguisticKnowledge loaded successfully");
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

    System.out.println("8) performing a recognition on the selection");
    final TextUIProgressBar progressBar = new TextUIProgressBar(System.out);
    progressBar.update(0, 100); // prints 0% status
    textRecognizer.process(textField, progressBar);
    progressBar.update(100, 100); // prints 100% status
    System.out.println("");
    System.out.println(" . recognition performed successfully");
    System.out.println("");

    System.out.println("9) retrieving the recognition result");
    final RecognitionResult textResult = (RecognitionResult)textField.getResult();
    System.out.println(" . result retrieved successfully");
    final CandidateIterator iterator = textResult.getCandidates();
    if (iterator.isAtEnd())
      System.out.println(" . sorry, unable to recognize the input");
    else
      System.out.println(" . input recognized as: \"" + iterator.getLabel() + "\"");
    System.out.println("");

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("10) cleaning-up: disposing of objects and disposing of the Engine");
    iterator.dispose();
    textResult.dispose();
    textRecognizer.dispose();
    selection.dispose();
    textField.dispose();
    content.dispose();
    page.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform a text recognition on a document");
  }

} // TextRecognition
