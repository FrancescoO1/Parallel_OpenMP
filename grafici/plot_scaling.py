import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import LogLocator, FuncFormatter

# Lettura dei CSV
bar_data = pd.read_csv('bar_chart_tempi.csv')
line_data = pd.read_csv('line_chart_speedup.csv')
throughput_data = pd.read_csv('throughput_chart.csv')
variability_data = pd.read_csv('variability_chart.csv')
overhead_data = pd.read_csv('overhead_chart.csv')
summary_data = pd.read_csv('riepilogo_medie.csv')

seq_row = summary_data[summary_data['Thread'] == 1].iloc[0]
throughput_seq_aos = seq_row['Throughput_AOS']
throughput_seq_soa = seq_row['Throughput_SOA']

# Stile (fallback come il tuo)
try:
    plt.style.use('seaborn-v0_8')
except OSError:
    try:
        plt.style.use('seaborn')
    except OSError:
        plt.style.use('default')

width = 0.35
num_fmt = FuncFormatter(lambda v, pos: f"{v:g}")

# Pre-compute per throughput assoluto (include 1 thread)
threads_thr_all = [1] + throughput_data['Thread'].tolist()
aos_thr_all = [throughput_seq_aos] + throughput_data['AOS_Throughput'].tolist()
soa_thr_all = [throughput_seq_soa] + throughput_data['SOA_Throughput'].tolist()
aos_std_all = [0] + throughput_data['AOS_StdDev'].tolist()
soa_std_all = [0] + throughput_data['SOA_StdDev'].tolist()

# -------- SLIDESHOW --------
def clear_axes(fig):
    fig.clf()
    ax = fig.add_subplot(111)
    return ax

def slide_tempi(fig):
    ax = clear_axes(fig)
    x = np.arange(len(bar_data['Thread']))
    ax.bar(x - width/2, bar_data['AOS_Tempo'], width,
           label='AOS', color='tab:blue', alpha=0.8,
           yerr=bar_data['AOS_StdDev'], capsize=5)
    ax.bar(x + width/2, bar_data['SOA_Tempo'], width,
           label='SOA', color='lightgreen', alpha=0.8,
           yerr=bar_data['SOA_StdDev'], capsize=5)
    ax.set_xlabel('Numero di Thread')
    ax.set_ylabel('Tempo di Esecuzione (s)')
    ax.set_title('Confronto Tempi di Esecuzione: AOS vs SOA')
    ax.set_xticks(x, bar_data['Thread'])
    ax.legend()
    ax.grid(True, alpha=0.3)

def slide_speedup(fig):
    ax = clear_axes(fig)
    ax.plot(line_data['Thread'], line_data['AOS_Speedup'], 'o-',
            label='AOS Speedup', linewidth=2, markersize=6)
    ax.plot(line_data['Thread'], line_data['SOA_Speedup'], 's-',
            label='SOA Speedup', linewidth=2, markersize=6)
    ax.plot(line_data['Thread'], line_data['Speedup_Teorico'], '--',
            label='Speedup Teorico', color='gray', linewidth=2, alpha=0.7)
    ax.set_xlabel('Numero di Thread'); ax.set_ylabel('Speedup')
    ax.set_title('Confronto Speedup: AOS vs SOA')
    ax.set_xscale('log', base=2); ax.set_yscale('log', base=2)
    ax.xaxis.set_major_locator(LogLocator(base=2))
    ax.yaxis.set_major_locator(LogLocator(base=2))
    ax.xaxis.set_major_formatter(num_fmt)
    ax.yaxis.set_major_formatter(num_fmt)
    ax.grid(True, alpha=0.3); ax.legend()

def slide_throughput(fig):
    ax = clear_axes(fig)
    x = np.arange(len(threads_thr_all))
    ax.bar(x - width/2, aos_thr_all, width, label='AOS',
           color='tab:blue', alpha=0.8, yerr=aos_std_all, capsize=5)
    ax.bar(x + width/2, soa_thr_all, width, label='SOA',
           color='lightgreen', alpha=0.8, yerr=soa_std_all, capsize=5)
    ax.set_xlabel('Numero di Thread'); ax.set_ylabel('Throughput (pwd/s)')
    ax.set_title('Confronto Throughput Assoluto')
    ax.set_xticks(x, threads_thr_all)
    ax.legend(); ax.grid(True, alpha=0.3)

def slide_variabilita(fig):
    ax = clear_axes(fig)
    ax.plot(variability_data['Thread'], variability_data['AOS_CV'],
            'o-', label='AOS CV%', linewidth=2, markersize=6)
    ax.plot(variability_data['Thread'], variability_data['SOA_CV'],
            's-', label='SOA CV%', linewidth=2, markersize=6)
    ax.set_xlabel('Numero di Thread'); ax.set_ylabel('Coefficiente di Variazione (%)')
    ax.set_title('Variabilità delle Misurazioni')
    ax.set_xscale('log', base=2)
    ax.xaxis.set_major_locator(LogLocator(base=2)); ax.xaxis.set_major_formatter(num_fmt)
    ax.grid(True, alpha=0.3); ax.legend()

def slide_overhead(fig):
    ax = clear_axes(fig)
    ax.plot(overhead_data['Thread'], overhead_data['AOS_Overhead'],
            'o-', label='AOS Overhead', linewidth=2, markersize=6)
    ax.plot(overhead_data['Thread'], overhead_data['SOA_Overhead'],
            's-', label='SOA Overhead', linewidth=2, markersize=6)
    ax.axhline(0, color='gray', linestyle='--', alpha=0.7, label='Nessun Overhead')
    ax.set_xlabel('Numero di Thread'); ax.set_ylabel('Overhead (%)')
    ax.set_title('Overhead di Parallelizzazione')
    ax.set_xscale('log', base=2)
    ax.xaxis.set_major_locator(LogLocator(base=2)); ax.xaxis.set_major_formatter(num_fmt)
    ax.grid(True, alpha=0.3); ax.legend()

# Lista delle slide aggiornata (rimuovo rapporti di scalabilità)
slides = [
    ("Tempi di Esecuzione", slide_tempi),
    ("Speedup", slide_speedup),
    ("Throughput Assoluto", slide_throughput),
    ("Variabilità (CV%)", slide_variabilita),
    ("Overhead di Parallelizzazione", slide_overhead),
]

fig = plt.figure(figsize=(12, 7))
idx = {'i': 0}

def draw():
    title, fn = slides[idx['i']]
    fn(fig)
    fig.suptitle(f"[{idx['i']+1}/{len(slides)}] {title}",
                 y=0.995, fontsize=11)
    fig.canvas.draw_idle()

def on_key(event):
    if event.key in ('right', 'down', ' ', 'pagedown'):
        idx['i'] = (idx['i'] + 1) % len(slides); draw()
    elif event.key in ('left', 'up', 'backspace', 'pageup'):
        idx['i'] = (idx['i'] - 1) % len(slides); draw()
    elif event.key in ('f', 'F'):
        # toggle full-screen (compatibile con diversi backend)
        try:
            fig.canvas.manager.full_screen_toggle()
        except Exception:
            try:
                fig.canvas.manager.window.showMaximized()
            except Exception:
                pass
    elif event.key in ('s', 'S'):
        fname = f"slide_{idx['i']+1:02d}.png"
        fig.savefig(fname, dpi=300, bbox_inches='tight')
        print(f"Salvata: {fname}")
    elif event.key in ('escape', 'q', 'Q'):
        plt.close(fig)

fig.canvas.mpl_connect('key_press_event', on_key)

# Prova ad aprire direttamente full-screen
try:
    fig.canvas.manager.full_screen_toggle()
except Exception:
    pass

draw()
plt.show()