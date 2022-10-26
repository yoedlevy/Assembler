# Assembler
התוכנית היא בעצם קומפיילר לשפת אסמבלי.
היא מקבלת קובץ אחד או יותר עם קוד באסמבלי, ועבור כל קובץ היא עוברת על הקוד, אם יש שגיאות בקוד התוכנית מתריעה על סוג השגיאה ועל השורה בה נמצאת השגיאה.
אם אין שגיאות בקוד, התוכנית יוצרת קובץ חדש עם הקוד לאחר פתיחת המקרואים (הקובץ יקרא כשם הקובץ המקורי אבל עם סיומת .am),
לאחר מכן היא מתרגמת את הקוד לשפת מכונה ושומרת את התרגום בקובץ חדש אחר (הפעם עם סיומת .obj),
ולבסוף אם קיימות תוויות entry ו-extern בקוד אז התוכנית תיצור קבצים חדשים עם סיומת .ent ו- .ext בהתאמה.
בקובץ .ent ישמרו שמות תוויות ה-entry וערכן בטבלת הסמלים,
בקובץ .ext ישמרו שמות תוויות ה-extern וכתובת בקוד המכונה בה יש קידוד של אופרנד המתייחס לסמל זה.
אם לא קיימות תוויות entry/extern אז התוכנית לא תיצור את הקבצים הנ"ל
