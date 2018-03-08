#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Mar  8 22:33:25 2018

@author: billzheng
"""

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as dates
from collections import Counter
 
  

  raw=[1398432160.5000000111, 1398432161.0000000111, 1398432161.0000000113,  1398432162.5000000111]
  values=[3,8,5,2]

  df = pd.DataFrame({'time' : raw})
  df.time = pd.Series.round(df.time)

  secs = dates.epoch2num(df.time)

  vc = pd.value_counts(df.time.values, sort=False)

  secs2 = dates.epoch2num(vc.index)


  print(secs)

  fig, ax = plt.subplots()
  # Plot the date using plot_date rather than plot
  #ax.plot_date(secs, values)
  #plt.plot(secs, values)


  plt.plot(secs, values)
  plt.plot(secs2, vc.values, drawstyle='steps-post')
  # Choose your xtick format string
  date_fmt = '%H:%M:%S.%f'

  # Use a DateFormatter to set the data to the correct format.
  date_formatter = dates.DateFormatter(date_fmt)
  ax.xaxis.set_major_formatter(date_formatter)

  #plt.plot(vc.index, vc.values)



  fig.autofmt_xdate()
  plt.grid()
  plt.show()
