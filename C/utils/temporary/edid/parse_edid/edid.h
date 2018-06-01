#ifndef EDID_H
#define EDID_H

struct edid_info {
	unsigned char version;			/* EDID version... */
	unsigned char revision;			/* ...and revision */

	unsigned char manufacturer[4];	/* Manufacturer */
	unsigned char product[14];		/* Monitor String */

	unsigned char serial_no[14];	/* Serial Number */
	unsigned char ascii[14];		/* ? */
	unsigned int model;			    /* Monitor Model */
	unsigned int serial;			/* Serial Number - Integer */
	unsigned int year;		    	/* Year manufactured */
	unsigned int week;		    	/* Week Manufactured */

    int   ratio_type;
	unsigned char hsize;           /* Maximum horizontal size (cm) */
	unsigned char vsize;           /* Maximum vertical size (cm) */
    float landscape_ratio;
    float portait_ratio;

    int err;                     /* parse errno */
};

int parse_ddc_edid(unsigned char *edid, struct edid_info *pinfo);

void dump_edidinfo(struct edid_info *pinfo);
#endif
