#include <libexif/exif-entry.h>
#include <libexif/exif-data.h>
#include <libexif/exif-ifd.h>
#include <libexif/exif-loader.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#include "JpegEncoderEXIF.h"

void exif_buf_free (exif_buffer * buf)
{
  free (buf->data);
  free (buf);
}

exif_buffer *exif_new_buf(unsigned char *data, unsigned int size)
{
    exif_buffer *res;
    
    res = (exif_buffer *) malloc(sizeof (exif_buffer));
    
    if( res == NULL)
        return NULL;
        
    res->data = (unsigned char *) malloc(size);
    if( res->data == NULL){
       free(res);
        
        return NULL;
    }
    
    memcpy ((void *) res->data, (void *) data, size);   
    res->size = size;
    
    return res;
}

void exif_entry_set_string (ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag, const char *s)
{
  ExifEntry *pE;

  pE = exif_entry_new ();
  exif_content_add_entry (pEdata->ifd[eEifd], pE);
  exif_entry_initialize (pE, eEtag);
  if (pE->data)
    free (pE->data);
  pE->components = strlen (s) + 1;
  pE->size = sizeof (char) * pE->components;
  pE->data = (unsigned char *) malloc (pE->size);
  if (!pE->data) {
    printf ("Cannot allocate %d bytes.\nTerminating.\n", (int) pE->size);
    exit (1);
  }
  strcpy ((char *) pE->data, (char *) s);
  exif_entry_fix (pE);
  exif_entry_unref (pE);
}

void exif_entry_set_undefined (ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag,
    exif_buffer * buf)
{
    ExifEntry *pE;

    pE = exif_entry_new ();
    exif_content_add_entry (pEdata->ifd[eEifd], pE);
    exif_entry_initialize (pE, eEtag);
    if (buf != NULL) {
        if (pE->data)
            free (pE->data);
        pE->components = buf->size;
        pE->size = buf->size;
        pE->data = (unsigned char *) malloc (pE->size);
        if (!pE->data) {
            printf ("Cannot allocate %d bytes.\nTerminating.\n", (int) pE->size);
            exit (1);
        }
        memcpy ((void *) pE->data, (void *) buf->data, buf->size);
    }
    exif_entry_fix (pE);
    exif_entry_unref (pE);
}

void  exif_entry_set_byte(ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag,
    ExifByte n)
{
    ExifEntry *pE;
    
    pE = exif_entry_new ();
    exif_content_add_entry (pEdata->ifd[eEifd], pE);
    exif_entry_initialize (pE, eEtag);
    if (pE->data) {
      pE->data = (unsigned char *) n;
    } else {
      printf ("ERROR: unallocated e->data Tag %d\n", eEtag);
    }
    exif_entry_fix (pE);
    exif_entry_unref (pE);
}

void exif_entry_set_short (ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag,
    ExifShort n)
{
  ExifEntry *pE;

  ExifByteOrder eO;

  pE = exif_entry_new ();
  exif_content_add_entry (pEdata->ifd[eEifd], pE);
  exif_entry_initialize (pE, eEtag);
  eO = exif_data_get_byte_order (pE->parent->parent);
  if (pE->data) {
    exif_set_short (pE->data, eO, n);
  } else {
    printf ("ERROR: unallocated e->data Tag %d\n", eEtag);
  }
  exif_entry_fix (pE);
  exif_entry_unref (pE);
}

void exif_entry_set_long (ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag,
    ExifLong n)
{
  ExifEntry *pE;

  ExifByteOrder eO;

  pE = exif_entry_new ();
  exif_content_add_entry (pEdata->ifd[eEifd], pE);
  exif_entry_initialize (pE, eEtag);
  eO = exif_data_get_byte_order (pE->parent->parent);
  if (pE->data) {
    exif_set_long (pE->data, eO, n);
  } else {
   printf ("ERROR: unallocated e->data Tag %d\n", eEtag);
  }
  exif_entry_fix (pE);
  exif_entry_unref (pE);
}

void exif_entry_set_rational (ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag,
    ExifRational r)
{
 ExifEntry *pE;

  ExifByteOrder eO;

  pE = exif_entry_new ();
  exif_content_add_entry (pEdata->ifd[eEifd], pE);
  exif_entry_initialize (pE, eEtag);
  eO = exif_data_get_byte_order (pE->parent->parent);
  if (pE->data) {
    exif_set_rational (pE->data, eO, r);
  } else {
    printf ("ERROR: unallocated e->data Tag %d\n", eEtag);
  }
  exif_entry_fix (pE);
  exif_entry_unref (pE);
}

void exif_entry_set_sbyte (ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag,
    ExifSByte n)
{
 ExifEntry *pE;

  char *pData;

  pE = exif_entry_new ();
  exif_content_add_entry (pEdata->ifd[eEifd], pE);
  exif_entry_initialize (pE, eEtag);
  pData = (char *) (pE->data);
  if (pData) {
    *pData = n;
  } else {
    printf ("ERROR: unallocated e->data Tag %d\n", eEtag);
  }
  exif_entry_fix (pE);
  exif_entry_unref (pE);
}

void exif_entry_set_sshort (ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag,
   ExifSShort n)
{
  ExifEntry *pE;

  ExifByteOrder eO;

  pE = exif_entry_new ();
  exif_content_add_entry (pEdata->ifd[eEifd], pE);
  exif_entry_initialize (pE, eEtag);
  eO = exif_data_get_byte_order (pE->parent->parent);
  if (pE->data) {
    exif_set_sshort (pE->data, eO, n);
  } else {
    printf ("ERROR: unallocated e->data Tag %d\n", eEtag);
  }
  exif_entry_fix (pE);
  exif_entry_unref (pE);
}

void exif_entry_set_slong (ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag,
    ExifSLong n)
{
  ExifEntry *pE;

 ExifByteOrder eO;

  pE = exif_entry_new ();
  exif_content_add_entry (pEdata->ifd[eEifd], pE);
  exif_entry_initialize (pE, eEtag);
  eO = exif_data_get_byte_order (pE->parent->parent);
  if (pE->data) {
    exif_set_slong (pE->data, eO, n);
  } else {
    printf ("ERROR: unallocated e->data Tag %d\n", eEtag);
  }
  exif_entry_fix (pE);
  exif_entry_unref (pE);
}

void exif_entry_set_srational (ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag,
    ExifSRational r)
{
  ExifEntry *pE;
  ExifByteOrder eO;

  pE = exif_entry_new ();
  exif_content_add_entry (pEdata->ifd[eEifd], pE);
  exif_entry_initialize (pE, eEtag);
  eO = exif_data_get_byte_order (pE->parent->parent);
  if (pE->data) {
    exif_set_srational (pE->data, eO, r);
  } else {
   printf ("ERROR: unallocated e->data Tag %d\n", eEtag);
  }
  exif_entry_fix (pE);
  exif_entry_unref (pE);
}

void exif_entry_set_gps_coord(ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag,
    ExifRational r1, ExifRational r2, ExifRational r3)
{
    ExifEntry *pE;
    ExifByteOrder eO;

    pE = exif_entry_new ();
    exif_content_add_entry (pEdata->ifd[eEifd], pE);
    exif_entry_gps_initialize(pE, eEtag);
    eO = exif_data_get_byte_order (pE->parent->parent);
    if (pE->data) {
        exif_set_rational (pE->data, eO, r1);
        exif_set_rational (pE->data + exif_format_get_size (pE->format), eO, r2);
        exif_set_rational (pE->data + 2 * exif_format_get_size (pE->format), eO, 
        r3);
    } else {
        printf ("ERROR: unallocated e->data Tag %d\n", eEtag);
    }
    exif_entry_fix (pE);
    /* exif_entry_dump (pE, 2); */
    exif_entry_unref (pE);
}

void exif_entry_set_gps_altitude(ExifData * pEdata, ExifIfd eEifd, ExifTag eEtag, ExifRational r1)
{
    ExifEntry *pE;
    ExifByteOrder eO;

    pE = exif_entry_new ();
    exif_content_add_entry (pEdata->ifd[eEifd], pE);
    exif_entry_gps_initialize(pE, eEtag);
    eO = exif_data_get_byte_order (pE->parent->parent);
    if (pE->data) {
        exif_set_rational (pE->data, eO, r1);
    } else {
        printf ("ERROR: unallocated e->data Tag %d\n", eEtag);
    }
    exif_entry_fix (pE);
    /* exif_entry_dump (pE, 2); */
    exif_entry_unref (pE);
}

exif_buffer *get_exif_buffer(void *gpsLocation)
{
    ExifData *pEd;
    exif_buffer *sEb;
    ExifRational sR;
    struct timeval sTv;
    struct tm *sTime;
   char *TimeStr;
    int res;

    sEb = (exif_buffer *) malloc (sizeof (exif_buffer));

    pEd = exif_data_new ();

    /* resolution */
    sR.numerator = 72;
    sR.denominator = 1;
    exif_entry_set_rational (pEd, EXIF_IFD_0, EXIF_TAG_X_RESOLUTION, sR);
    exif_entry_set_rational (pEd, EXIF_IFD_0, EXIF_TAG_Y_RESOLUTION, sR);
    exif_entry_set_short (pEd, EXIF_IFD_0, EXIF_TAG_RESOLUTION_UNIT, 2); /* inches */
    exif_entry_set_short (pEd, EXIF_IFD_0, EXIF_TAG_YCBCR_POSITIONING, 1); /* centered */
    exif_entry_set_short(pEd, EXIF_IFD_0, EXIF_TAG_YCBCR_SUB_SAMPLING, 0);

    /* Exif version */
    exif_entry_set_undefined (pEd, EXIF_IFD_EXIF, EXIF_TAG_EXIF_VERSION, NULL);

    /* flashpix version */
    exif_entry_set_undefined(pEd, EXIF_IFD_EXIF, EXIF_TAG_FLASH_PIX_VERSION, NULL);
    
    /* file source */
    exif_entry_set_undefined(pEd, EXIF_IFD_EXIF, EXIF_TAG_FILE_SOURCE, NULL);
    
    /* file name */
    exif_entry_set_undefined(pEd, EXIF_IFD_EXIF, EXIF_TAG_DOCUMENT_NAME, NULL);
    
    /* scene type */
    exif_entry_set_undefined(pEd, EXIF_IFD_EXIF, EXIF_TAG_SCENE_TYPE, NULL);

    /* Color Components */
    exif_entry_set_undefined(pEd, EXIF_IFD_EXIF, EXIF_TAG_COMPONENTS_CONFIGURATION, NULL);
    
    /* Bits per sample */
    exif_entry_set_undefined (pEd, EXIF_IFD_0, EXIF_TAG_BITS_PER_SAMPLE, NULL);
    
    /* Color space */
   exif_entry_set_short (pEd, EXIF_IFD_EXIF, EXIF_TAG_COLOR_SPACE, 1);
    
    /* Interoperability index */
    exif_entry_set_string(pEd, EXIF_IFD_INTEROPERABILITY, EXIF_TAG_INTEROPERABILITY_INDEX, "R98");

    /* time */
    /* this sould be last resort */
    res = gettimeofday (&sTv, NULL);
    sTime = localtime (&sTv.tv_sec);
    if (res == 0 && sTime != NULL) {
        TimeStr = (char *) malloc(20);/* No data for secondary sensor */
    
        snprintf(TimeStr, 20, "%04d:%02d:%02d %02d:%02d:%02d",
                 sTime->tm_year + 1900,
                 sTime->tm_mon + 1,
                 sTime->tm_mday,
                 sTime->tm_hour,
                 sTime->tm_min,
                 sTime->tm_sec
                );
        exif_entry_set_string (pEd, EXIF_IFD_0, EXIF_TAG_DATE_TIME, TimeStr);
        exif_entry_set_string (pEd, EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_ORIGINAL, TimeStr);
        exif_entry_set_string (pEd, EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_DIGITIZED, TimeStr);
        snprintf(TimeStr, 20, "%06d", (int) sTv.tv_usec);
        exif_entry_set_string (pEd, EXIF_IFD_EXIF, EXIF_TAG_SUB_SEC_TIME, TimeStr);
        exif_entry_set_string (pEd, EXIF_IFD_EXIF, EXIF_TAG_SUB_SEC_TIME_ORIGINAL, TimeStr);
        exif_entry_set_string (pEd, EXIF_IFD_EXIF, EXIF_TAG_SUB_SEC_TIME_DIGITIZED, TimeStr);
        free (TimeStr);
    } else {
        printf ("Error in time recognition. res: %d sTime: %p\n%s\n", res, sTime, strerror(errno));
    }

    exif_entry_set_short (pEd, EXIF_IFD_1, EXIF_TAG_COMPRESSION, 6); /* JPEG */
    exif_entry_set_long (pEd, EXIF_IFD_1, EXIF_TAG_JPEG_INTERCHANGE_FORMAT, 0xFFFFFFFF);
    exif_entry_set_long (pEd, EXIF_IFD_1, EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH, 0xFFFFFFFF);

    if ( NULL != gpsLocation ) {
        ExifRational r1, r2, r3;
        gps_data *gps = (gps_data *) gpsLocation;
        
        /* gps data */
        r1.denominator = 1;
        r2.denominator = 1;
        r3.denominator = 1;
        
        r1.numerator = gps->longDeg;
        r2.numerator = gps->longMin;
        r3.numerator = gps->longSec;
        
        exif_entry_set_gps_coord(pEd, EXIF_IFD_GPS, (ExifTag) EXIF_TAG_GPS_LONGITUDE, r1, r2, r3);
        
        r1.numerator = gps->latDeg;
        r2.numerator = gps->latMin;
        r3.numerator = gps->latSec;
        
        exif_entry_set_gps_coord(pEd, EXIF_IFD_GPS, (ExifTag) EXIF_TAG_GPS_LATITUDE, r1, r2, r3);
    
        r1.numerator = gps->altitude;
        exif_entry_set_gps_altitude(pEd, EXIF_IFD_GPS, (ExifTag) EXIF_TAG_GPS_ALTITUDE, r1);

    }

    /* copy data to our buffer */
    exif_data_save_data (pEd, &sEb->data, &sEb->size);

    /* destroy exif structure */
    exif_data_unref(pEd);

    return sEb;
}

