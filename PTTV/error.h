/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __ERROR_H__
#define __ERROR_H__

typedef enum {
  ERR_ENDOFCOMMENT,
  ERR_IDENTTOOLONG,
  ERR_INVALIDCHARCONSTANT,
  ERR_INVALIDSYMBOL,
  ERR_NUMBERTOOLONG,
} ErrorCode;


#define ERM_ENDOFCOMMENT "End of comment expected!"//Đang đọc dở Token hay ô tô mát chưa đến trạng thái kết thúc được mà hết mảng ký tự.
#define ERM_IDENTTOOLONG "Identification too long!"// max 16 ký tự
#define ERM_INVALIDCHARCONSTANT "Invalid const char!" //không tìm thấy ký tự đóng ngoặc của hằng ký tự ' đúng phải là ''
#define ERM_INVALIDSYMBOL "Invalid symbol!" // ký tự không hợp lệ vd %,&,@

void error(ErrorCode err, int lineNo, int colNo);

#endif
