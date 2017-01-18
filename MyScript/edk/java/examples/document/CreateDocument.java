package document;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.document.Document;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import common.CFG;

/**
 * The <code>CreateDocument/code> example code shows you the various ways of
 * creating and loading a MyScript Document.
 */
public class CreateDocument
{
  /**
   * The working folder and document files used in this example. (Note that
   * you can rename any notes document as a zip file to watch inside it.)
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  private static final String EXISTING_DOCUMENT   = CFG.DATA_DIR + "/example4.notes";
  private static final String NEW_DOCUMENT        = CFG.OUTPUT_DIR + "/new.notes";

  /**
   * Main entry point for the <code>CreateDocument</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (EXISTING_DOCUMENT == null || EXISTING_DOCUMENT.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing EXISTING_DOCUMENT static member variable of CreateDocument class");
      System.exit(-1);
    }

    if (NEW_DOCUMENT == null || NEW_DOCUMENT.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing NEW_DOCUMENT static member variable of CreateDocument class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of CreateDocument class");
      System.exit(-1);
    }
    
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) loading an existing Document using its filename and the path of its temporary folder");
    final Document existingDocument1 = Document.create(engine, EXISTING_DOCUMENT, TEMPORARY_DIRECTORY, false, false);
    System.out.println(" . " + new File(EXISTING_DOCUMENT).getCanonicalPath() + " loaded successfully");
    System.out.println();

    System.out.println("3) creating a new Document");
    System.out.println(" . creating a Document can be done the same way as when loading a Document");
    System.out.println("   using its filename and the path of its temporary folder:");
    System.out.println("   if the file name specified points to a non existing directory, a new directory is created");
    System.out.println("   if a Document already matches the file name specified, it is overwritten");
    System.out.println("   if no Document matches the file name specified, a new Document is created");
    final Document newDocument = Document.create(engine, NEW_DOCUMENT, TEMPORARY_DIRECTORY, true, true);
    System.out.println(" . new Document created successfully");
    System.out.println();

    System.out.println("4) loading an existing Document with voLoadObjectFromFile()");
    System.out.println(" . loading the Document from its file name");
    System.out.println("   . if file name is not found, an error is triggered");
    final Document existingDocument2 = (Document)EngineObject.load(engine, EXISTING_DOCUMENT);
    System.out.println(" . setting the Document temporary directory");
    existingDocument2.setTemporaryDirectory(TEMPORARY_DIRECTORY);
    System.out.println(" . Document loaded successfully");
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("5) cleaning-up: disposing of objects and disposing of the Engine");
    existingDocument2.dispose();
    existingDocument1.dispose();
    newDocument.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to create and load Documents");
  }

} // CreateDocument
