from pymongo import MongoClient
import sys
from PyQt5.QtWidgets import * 
from PyQt5.QtGui import * 
from PyQt5.QtCore import QTimer,QDateTime
from subprocess import call
import datetime

class List(QWidget):

    def __init__(self, col):
        super().__init__()
        self.timer=QTimer()
        self.timer.timeout.connect(self.updateList)
        self.initUI()
        self.colletion = col
        
    def updateList(self):
        self.listWidget.clear()
        self.listWidget.addItem("car\t\tmNbr\t\tDifference\t\ttimespamp")
        database_entries = self.colletion.find().sort("_id", 1)
        length = database_entries.count()
        for i in range(10 if length >=10 else length):
            entry = database_entries[length-i-1]
            line = str(entry["name"]) + "\t\t" + str(entry["measurement_number"])+ "\t\t" + str(entry["difference"]/1000)+ "\t\t" + str(datetime.datetime.fromtimestamp(entry["drive_through_timestamp"]/1000.0).isoformat(' '))[11:-3]
            self.listWidget.addItem(line)
            if i >=10:
                break
        
    def startTimer(self):
        self.timer.start(1000)
        
    def initUI(self):

        vbox = QVBoxLayout(self)
        
        self.listWidget = QListWidget()
        self.setLayout(vbox)
        self.listWidget.setFont(QFont('Arial', 22))

        vbox.addWidget(self.listWidget)
        
        hbox = QHBoxLayout(self)
        vbox.addLayout(hbox)
        
        self.closeButton = QPushButton(self)
        self.closeButton.setText("Close")          #text
        self.closeButton.setShortcut('Ctrl+D')  #shortcut key
        self.closeButton.clicked.connect(self.close)
        self.closeButton.setToolTip("Close the App") #Tool tip
        hbox.addWidget(self.closeButton)
        
        #self.shutdownButton = QPushButton(self)
        #self.shutdownButton.setText("Shutdown RallyMaster")          #text
        #self.shutdownButton.clicked.connect(call("sudo shutdown -h now", shell=True))
        #self.shutdownButton.setToolTip("Shutdown RallyMaster") #Tool tip
        #hbox.addWidget(self.shutdownButton)
        
        
        self.showFullScreen()
        self.setWindowTitle('Rally Timer')
        self.startTimer()
        self.show()

def main():
    connection = MongoClient('localhost', 27017)
    db = connection.rallyDB
    collection = db.driveThroughs
    
    app = QApplication(sys.argv)
    ex = List(collection)
    sys.exit(app.exec_())
    

if __name__ == '__main__':
    main()
