import sys;
from reportlab.graphics.shapes import *
from reportlab.graphics import renderPDF
from reportlab.graphics.charts.lineplots import LinePlot

def main():
	f = file("Translate1-2004-01-09-g(9.8).txt")
	timex = []
	timey = []
	timez = []
	for line in f:		
		list = line.split(' ')
		timex.append((float(list[0]), float(list[1])))
		timey.append((float(list[0]), float(list[2])))

	drawing = Drawing(400,600)
	
	max_time, _ = timex[len(timex)-1]
	_, max_x = timex[len(timex)-1]	
	
	lineplot_x = LinePlot()	
	lineplot_x.x = max_time+1
	##lineplot_x.x = 50
	lineplot_x.y = max_x+1
	##lineplot_x.y = 50
	lineplot_x.height = 125
	lineplot_x.width = 300	
	lineplot_x.data = [timex]
	drawing.add(lineplot_x)
	
	_, max_y = timey[len(timey)-1]
	lineplot_y = LinePlot()
	lineplot_y.x = max_time+1
	lineplot_y.y = max_y+1
	lineplot_y.height = 125
	lineplot_y.width = 300
	lineplot_y.data = [timey]
	##drawing.add(lineplot_y)
	
##	renderPDF.drawToFile(drawing, 'XVal1VsTime-2004-01-09-g(9.8).pdf', 'XvsTime')
##	_, max_y = timex[len(timey)-1]
##	lineplot_y = LinePlot()	
	
	##write data to files
	file_x = file("xpos.txt", 'w')
	for item in timex:
		str = item.__str__()
		str = str.replace('(', '')
		str = str.replace(')', '')
		str = str.replace(',', '')
		file_x.write(str + '\n')
		
	file_y = file("ypos.txt", 'w')
	for item in timey:
		str = item.__str__()
		str = str.replace('(', '')
		str = str.replace(')', '')
		str = str.replace(',', '')
		file_y.write(str + '\n')
		
if __name__ == '__main__':
	main()
