function doGet() {
  var sheet = SpreadsheetApp.openById("10bZ3OzHxoFFNzhOfY0LLyThDswl0cW_ckqPFSmUXV28").getActiveSheet();
  var data = sheet.getDataRange().getValues(); 
  var jsonData = JSON.stringify(data);
  console.log(jsonData);
  return ContentService.createTextOutput(jsonData).setMimeType(ContentService.MimeType.JSON);
}
