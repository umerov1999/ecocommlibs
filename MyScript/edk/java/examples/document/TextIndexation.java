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
import com.myscript.inksearch.TextIndexer;
import com.myscript.text.Resource;
import common.CFG;
import common.TextUIProgressBar;

/**
 * The <code>TextIndexation</code> example code shows you how to perform a text
 * indexation on a document.
 */
public class TextIndexation
{
  /**
   * The temporary directory used in this example.
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  
  /**
   * The document file opened to load its ink. (Note that you can rename any
   * notes document as a zip file to watch inside it.)
   */
  private static final String DOCUMENT_FILE = CFG.DATA_DIR + "/example2.notes";
  
  /**
   * The document file created to store the indexes. (Note that you can rename
   * any notes document as a zip file to watch inside it.)
   */
  private static final String DOCUMENT_COPY_FILE = TEMPORARY_DIRECTORY + "/example2_copy.notes";

  /**
   * Main entry point for the <code>TextIndexation</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of TextIndexation class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of TextIndexation class");
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
    final Page page0 = document.getPageAt(0);
    System.out.println(" . first page retrieved successfully");
    final Content[] contentHandler = new Content[1];
    page0.getContent(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to load page content");
        contentHandler[0] = (Content)value.acquire();
      }
    });
    page0.sync(); // wait for getContent() to complete
    final Content content0 = contentHandler[0];
    System.out.println(" . first page's content retrieved successfully");
    System.out.println("");

    System.out.println("4) adding a new text field to the content of the first page");
    ContentField textField = content0.addNewField(ContentFieldType.TEXT, "myTextField");
    System.out.println(" . text field added successfully");
    System.out.println("");

    System.out.println("5) selecting all the first page");
    PageSelection selection = PageSelection.create(page0);
    selection.selectAll();
    System.out.println(" . selection done successfully");
    System.out.println("");

    System.out.println("6) adding the selection to the text field");
    textField.addItems(selection);
    System.out.println(" . selection added to the text field successfully");
    System.out.println("");

    System.out.println("7) creating a new TextIndexer");
    TextIndexer textIndexer = TextIndexer.create(engine);
    System.out.println(" . TextIndexer created successfully");
    System.out.println("");

    System.out.println("8) configuring the TextIndexer");
    final Resource ak = (Resource)EngineObject.load(engine, CFG.getAkCurRes());
    System.out.println(" . AlphabetKnowledge loaded successfully");
    final Resource lk = (Resource)EngineObject.load(engine, CFG.getLkTextRes());
    System.out.println(" . LinguisticKnowledge loaded successfully");
    textIndexer.attach(ak);
    textIndexer.attach(lk);
    Resource lkEnglishSecondary = null;
    if (!CFG.getLang().contains("en"))
    {
      final String path;

      if (CFG.getLang().contains("zh") || CFG.getLang().contains("ja") || CFG.getLang().contains("ko"))
        path = CFG.getLkEnglishSecondary2Res();
      else if (CFG.getLang().contains("hi"))
        path = CFG.getLkEnglishSecondary3Res();
      else
        path = CFG.getLkEnglishSecondary1Res();

      lkEnglishSecondary = (Resource)EngineObject.load(engine, path);
      System.out.println(" . Secondary English LinguisticKnowledge resource loaded successfully");
      textIndexer.attach(lkEnglishSecondary);
      lkEnglishSecondary.dispose();
    }
    System.out.println(" . AlphabetKnowledge and LinguisticKnowledge resources attached to the indexer successfully");
    System.out.println("");

    System.out.println("9) performing an indexation on the selection (first page)");
    TextUIProgressBar progressBar = new TextUIProgressBar(System.out);
    progressBar.update(0, 100); // prints 0% status
    textIndexer.process(textField, progressBar);
    progressBar.update(100, 100); // prints 100% status
    System.out.println("");
    System.out.println(" . indexation performed successfully");
    System.out.println("");

    System.out.println("10) disposing of all indexation objects");
    textIndexer.dispose();
    selection.dispose();
    textField.dispose();
    System.out.println("");
    
    System.out.println("11) getting the second page of the Document and its content");
    final Page page1 = document.getPageAt(1);
    document.sync();
    System.out.println(" . second page retrieved successfully");
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
    final Content content1 = contentHandler[0];
    System.out.println(" . second page's content retrieved successfully");
    System.out.println("");

    System.out.println("12) adding a new text field to the content of the second page");
    textField = content1.addNewField(ContentFieldType.TEXT, "myTextField");
    System.out.println(" . text field added successfully");
    System.out.println("");

    System.out.println("13) selecting all the second page");
    selection = PageSelection.create(page1);
    selection.selectAll();
    System.out.println(" . selection done successfully");
    System.out.println("");

    System.out.println("14) adding the selection to the text field");
    textField.addItems(selection);
    System.out.println(" . selection added to the text field successfully");
    System.out.println("");

    System.out.println("15) creating and configuring a new TextIndexer");
    textIndexer = TextIndexer.create(engine);
    System.out.println(" . TextIndexer created successfully");
    textIndexer.attach(ak);
    textIndexer.attach(lk);
    System.out.println(" . AlphabetKnowledge and LinguisticKnowledge resources attached to the indexer successfully");
    System.out.println("");

    System.out.println("16) performing an indexation on the selection (second page)");
    progressBar = new TextUIProgressBar(System.out);
    progressBar.update(0, 100); // prints 0% status
    textIndexer.process(textField, progressBar);
    progressBar.update(100, 100); // prints 100% status
    System.out.println("");
    System.out.println(" . indexation done successfully");
    System.out.println("");

    System.out.println("17) releasing all indexation objects");
    textIndexer.dispose();
    selection.dispose();
    textField.dispose();
    System.out.println("");

    System.out.println("18) saving a copy of the Document");
    document.saveAs(DOCUMENT_COPY_FILE, new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to save copy of the document");
      }
    });
    document.sync(); // wait for saveAs() to complete
    System.out.println(" . " + new File(DOCUMENT_COPY_FILE).getCanonicalPath() + " saved successfully");
    System.out.println("");

    System.out.println("19) cleaning-up: releasing all created objects and destroying the Engine");
    ak.dispose();
    lk.dispose();
    if (lkEnglishSecondary != null)
      lkEnglishSecondary.dispose();
    content1.dispose();
    content0.dispose();
    page1.dispose();
    page0.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println("");

    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform a text indexation on a document");
  }

} // TextIndexation
