package document;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.document.AsyncCommand;
import com.myscript.document.Document;
import com.myscript.document.IAsyncCommandHandler;
import com.myscript.document.Page;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import common.CFG;

/**
 * The <code>LoadSaveDocument/code> example code shows you how to perform IO
 * operations on a document (loading and saving).
 */
public class LoadSaveDocument
{
  /**
   * The working folder and document files used in this example. (Note that you
   * can rename any notes document as a zip file to watch inside it.)
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  private static final String DOCUMENT_FILE       = CFG.DATA_DIR + "/example1.notes";
  private static final String DOCUMENT_COPY_FILE  = TEMPORARY_DIRECTORY + "/example1_copy.notes";

  /**
   * Main entry point for the <code>LoadSaveDocument</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of LoadSaveDocument class");
      System.exit(-1);
    }

    if (DOCUMENT_COPY_FILE == null || DOCUMENT_COPY_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_COPY_FILE static member variable of LoadSaveDocument class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of LoadSaveDocument class");
      System.exit(-1);
    }
    
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) loading an existing Document");
    Document document = (Document)EngineObject.load(engine, DOCUMENT_FILE);
    document.setTemporaryDirectory(TEMPORARY_DIRECTORY);
    System.out.println(" . " + new File(DOCUMENT_FILE).getCanonicalPath() + " loaded successfully");
    System.out.println();

    System.out.println("3) getting the number of pages of the document");
    int pageCount = document.getPageCount();
    if (pageCount != 2)
    {
      System.err.println(" . the Document doesn't contain 2 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("4) adding a new page to the Document");
    Page page = document.addPage();
    page.dispose();
    pageCount = document.getPageCount();
    if (pageCount != 3)
    {
      System.err.println(" . the Document doesn't contain 3 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("5) saving a copy of the Document");
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
    System.out.println();

    System.out.println("6) closing the Document");
    document.dispose();
    System.out.println(" . Document closed successfully");
    System.out.println();

    System.out.println("7) loading the copy of the Document");
    document = (Document)EngineObject.load(engine, DOCUMENT_COPY_FILE);
    document.setTemporaryDirectory(TEMPORARY_DIRECTORY);
    System.out.println(" . " + new File(DOCUMENT_COPY_FILE).getCanonicalPath() + " loaded successfully");
    pageCount = document.getPageCount();
    if (pageCount != 3)
    {
      System.err.println(" . the Document doesn't contain 3 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("8) adding a new page to the Document");
    page = document.addPage();
    page.dispose();
    pageCount = document.getPageCount();
    if (pageCount != 4)
    {
      System.err.println(" . the Document doesn't contain 4 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("9) saving a temporary copy of the Document");
    document.saveToTemp(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to save the document into the temp directory");
      }
    });
    document.sync(); // wait for saveToTemp() to complete
    System.out.println(" . temporary document saved successfully");
    System.out.println();

    System.out.println("10) discarding changes and closing the Document");
    document.discardChanges(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to discard the changes of the document");
      }
    });
    document.sync(); // wait for discardChanges() to complete
    document.dispose();
    System.out.println(" . Document closed successfully");
    System.out.println();

    System.out.println("11) re-loading the copy of the Document");
    document = (Document)EngineObject.load(engine, DOCUMENT_COPY_FILE);
    document.setTemporaryDirectory(TEMPORARY_DIRECTORY);
    System.out.println(" . " + new File(DOCUMENT_COPY_FILE).getCanonicalPath() + " loaded successfully");
    pageCount = document.getPageCount();
    if (pageCount != 3)
    {
      System.err.println(" . the Document doesn't contain 3 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("12) adding a new page in the Document");
    page = document.addPage();
    page.dispose();
    pageCount = document.getPageCount();
    if (pageCount != 4)
    {
      System.err.println(" . the Document doesn't contain 4 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("13) re-saving the copy of the Document");
    document.save(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to save the document");
      }
    });
    document.sync(); // wait for save() to complete
    System.out.println(" . Document re-saved successfully");
    System.out.println();

    System.out.println("14) adding a new page in the Document");
    page = document.addPage();
    page.dispose();
    pageCount = document.getPageCount();
    if (pageCount != 5)
    {
      System.err.println(" . the Document doesn't contain 5 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("15) closing the Document");
    document.dispose();
    System.out.println(" . Document closed successfully");
    System.out.println();

    System.out.println("16) re-loading the copy of the Document");
    document = (Document)EngineObject.load(engine, DOCUMENT_COPY_FILE);
    document.setTemporaryDirectory(TEMPORARY_DIRECTORY);
    System.out.println(" . " + new File(DOCUMENT_COPY_FILE).getCanonicalPath() + " loaded successfully");
    pageCount = document.getPageCount();
    if (pageCount != 4)
    {
      System.err.println(" . the Document doesn't contain 4 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("17) testing and enabling the auto save on the Document");
    boolean enableAutoSave = document.getAutoSave();
    if (enableAutoSave)
    {
      System.err.println(" . the Document auto save feature is not disabled");
      System.exit(-1);
    }
    System.out.println(" . Auto save feature is disabled");
    enableAutoSave = true;
    document.setAutoSave(enableAutoSave);
    System.out.println(" . Auto save feature updated successfully");
    enableAutoSave = document.getAutoSave();
    if (!enableAutoSave)
    {
      System.err.println(" . the Document auto save feature is not enabled");
      System.exit(-1);
    }
    System.out.println(" . Auto save feature is enabled");
    System.out.println();

    System.out.println("18) adding a new page in the Document");
    page = document.addPage();
    page.dispose();
    pageCount = document.getPageCount();
    if (pageCount != 5)
    {
      System.err.println(" . the Document doesn't contain 5 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("19) closing the Document");
    document.dispose();
    System.out.println(" . Document closed successfully");
    System.out.println(" . Modifications were automatically saved in the temporary directory");
    System.out.println();

    System.out.println("20) re-loading the copy of the Document using its filename and the path of its temporary folder");
    document = Document.create(engine, DOCUMENT_COPY_FILE, TEMPORARY_DIRECTORY, false, false);
    System.out.println(" . " + new File(DOCUMENT_COPY_FILE).getCanonicalPath() + " loaded successfully");
    pageCount = document.getPageCount();
    if (pageCount != 5)
    {
      System.err.println(" . the Document doesn't contain 5 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    System.out.println("21) discarding changes and closing the Document");
    document.discardChanges(new IAsyncCommandHandler()
    {
      public void onCompletion(AsyncCommand cmd, boolean success,
                               EngineObject value)
      {
        if (!success)
          throw new RuntimeException("failed to discard the changes of the document");
      }
    });
    document.sync(); // wait for discardChanges() to complete
    document.dispose();
    System.out.println(" . Document closed successfully");
    System.out.println();

    System.out.println("22) re-loading the copy of the Document using its filename and the path of its temporary folder");
    document = Document.create(engine, DOCUMENT_COPY_FILE, TEMPORARY_DIRECTORY);
    System.out.println(" . " + new File(DOCUMENT_COPY_FILE).getCanonicalPath() + " loaded successfully");
    pageCount = document.getPageCount();
    if (pageCount != 4)
    {
      System.err.println(" . the Document doesn't contain 4 pages");
      System.exit(-1);
    }
    System.out.println(" . the Document contains " + pageCount + " page(s)");
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("23) cleaning-up: disposing of objects and disposing of the Engine");
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform IO operations on a document (loading and saving)");
  }

} // LoadSaveDocument
