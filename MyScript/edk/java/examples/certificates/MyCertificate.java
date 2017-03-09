package certificates;

/**
 * The <code>MyCertificate</code> class provides the bytes of the certificate used
 * to identify you as a legitimate MyScript Engine user.
 */
public final class MyCertificate
{
  /**
   * Returns the bytes of the certificate.
   * 
   * @return the bytes of the certificate.
   */
  public static final byte[] getBytes()
  {
    System.err.println("/!\\ please configure your certificate before launching the example programs");
    System.exit(-1);
    return null;
  }

} // MyCertificate
