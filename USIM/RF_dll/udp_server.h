#define DEFAULT_PORT	"12345"
#define DEFAULT_IP		"127.0.0.1"

#define WSA_ACCEPT   (WM_USER + 0)
#define WSA_READ     (WM_USER + 1)

#define IDM_HOSTNAME        102
#define IDM_LISTEN          103
#define IDM_CONNECT         104
#define IDM_ALISTEN         105
#define IDM_CANCEL          106
#define IDM_TLISTEN         107
#define IDM_SENDTCP         108
#define IDM_CONNECTANDSEND  109
#define IDM_ACCEPTEX        110

#define MAX_PENDING_CONNECTS 4  // The backlog allowed for listen()
#define MY_MSG_LENGTH       80  // msg buffer sent back and forth
#define NO_FLAGS_SET         0  // Used with recv()/send()

char szBuff[80];				// Temp buffer - used to pass strings
								// to and from dialog boxes, etc
u_short portno;					// Which tcp port are we going to use?
//CString ip_address = DEFAULT_IP;
//CString port = DEFAULT_PORT;

//#define DEFAULT_PROTO	SOCK_DGRAM // UDP
